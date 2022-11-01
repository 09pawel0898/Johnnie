#include "EnginePCH.hpp"

#include "StaticMesh.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/Renderer.hpp"
#include "Scene/Entities/CoreActor.hpp"
#include "Scene/SceneDeleates.hpp"
#include "Core/Debug/ProfileMacros.hpp"

#include "AssimpHelpers.hpp"

namespace Engine
{
   AStaticMesh::AStaticMesh(std::string const& FilePath, OnStaticMeshAsyncLoadingFinishedDelegate OnLoadingFinished, glm::vec3 const& WorldLocation)
       :    Actor(WorldLocation),
            m_ModelFilePath(FilePath)
   {
       OnAsyncLoadingFinishedDelegate = MoveTemp(OnLoadingFinished);
       m_bScheduleModelLoadOnConstruct = true;
   }

    AStaticMesh::AStaticMesh(std::vector<TSharedPtr<Mesh>>&& SubMeshes, glm::vec3 const& WorldLocation)
        :   Actor(WorldLocation)
    {
        m_bIsModelImported = true;
        m_SubMeshes = MoveTemp(SubMeshes);

        InitializeMaterialSlots(MaterialsEvaluateMethod::OnePerSubmesh);
    }

    AStaticMesh::~AStaticMesh()
    {}

    void AStaticMesh::ImportModel(std::string_view FilePath)
    {
        m_ModelImporter = AssetImporter::Create();

        const aiScene* scene = m_ModelImporter->GetImporter().ReadFile( FilePath.data(),
                aiProcess_Triangulate 
            |   aiProcess_OptimizeMeshes 
            |   aiProcess_RemoveRedundantMaterials
            |   aiProcess_CalcTangentSpace
            |   aiProcess_PreTransformVertices);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG(Assimp, Error, "{0}", m_ModelImporter->GetImporter().GetErrorString());
            return;
        }
        m_Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
        
        InitializeMaterialSlots(MaterialsEvaluateMethod::FromModelImporter);
        ProcessNode(scene->mRootNode, scene);
        
        m_bIsModelImported = true;    
    }
    
    void AStaticMesh::ProcessNode(aiNode* Node, const aiScene* Scene)
    {
        for (uint8_t i = 0; i < Node->mNumMeshes; i++)
        {    
            aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];

            TSharedPtr<Mesh> subMesh = ProcessMesh(mesh, Scene);
            subMesh->SetStaticMeshOwner(shared_from_this());
            subMesh->GetMeshStatistics().TrisCount = mesh->mNumFaces;

            m_SubMeshes.emplace_back(MoveTemp(subMesh));
        }
    
        for (uint8_t i = 0; i < Node->mNumChildren; i++)
        {
            ProcessNode(Node->mChildren[i], Scene);
        }
    }
    
    TSharedPtr<Mesh> AStaticMesh::ProcessMesh(aiMesh* Mesh_, const aiScene* Scene)
    {
        std::vector<RHIVertex> vertices;
        std::vector<uint32_t> indices;
    
        vertices.reserve(Mesh_->mNumVertices);
        indices.reserve(Mesh_->mNumFaces * 3);
        
        // Process vertices //
        for (uint32_t i = 0; i < Mesh_->mNumVertices; i++)
        {
            RHIVertex vertex;
    
            aiVector3D const& vertexPosition = Mesh_->mVertices[i];
            vertex.Position = { vertexPosition.x, vertexPosition.y, vertexPosition.z };
            
            if (Mesh_->mNormals)
            {
                aiVector3D const& normal = Mesh_->mNormals[i];
                vertex.Normal = { normal.x, normal.y, normal.z };
            }
            
            if (Mesh_->mTangents)
            {
                aiVector3D const& tangent = Mesh_->mTangents[i];
                vertex.Tangent = { tangent.x, tangent.y, tangent.z };
            }
            
            if (Mesh_->mTextureCoords[0])
            {
                aiVector3D const& textureCoords = Mesh_->mTextureCoords[0][i];
                vertex.TexUV = { textureCoords.x, textureCoords.y };
            }
            else
            {
                vertex.TexUV = { 0.f, 0.f };
            }
            vertices.emplace_back(MoveTemp(vertex));
        }
    
        // Process indices //
        for (uint32_t i = 0; i < Mesh_->mNumFaces; i++)
        {
            aiFace const& face = Mesh_->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
            {
                indices.emplace_back(face.mIndices[j]);
            }
        }
        
        TSharedPtr<Mesh> subMesh = MakeShared<Mesh>(MoveTemp(vertices),
                                                    MoveTemp(indices),
                                                    true);

        // Process materials //
        if (Mesh_->mMaterialIndex >= 0)
        {
            subMesh->SetMaterialIndex(Mesh_->mMaterialIndex);
        }

        return subMesh;
    }

    void AStaticMesh::InitializeMaterialSlots(MaterialsEvaluateMethod EvaluateMethod)
    {
        m_Materials.clear();

        if (EvaluateMethod == MaterialsEvaluateMethod::FromModelImporter)
        {
            m_MaterialProcessed.clear();

            const aiScene* Scene = m_ModelImporter->GetScene();

            if (Scene != nullptr)
            {
                Emplace_N_MaterialSlots(Scene->mNumMaterials);
            }
            else
            {
                CheckMsg(false, "Scene was nullptr, can't parse materials info for initialization.");
            }
        }
        else if (EvaluateMethod == MaterialsEvaluateMethod::OnePerSubmesh)
        {
            Emplace_N_MaterialSlots((uint8_t)m_SubMeshes.size());
            
            for (uint8_t idx = 0; idx < m_SubMeshes.size(); idx++)
            {
                m_SubMeshes[idx]->SetMaterialIndex(idx);
            }
        }
    }

    void AStaticMesh::Emplace_N_MaterialSlots(uint8_t N)
    {
        m_Materials.reserve(N);
        m_MaterialProcessed.reserve(N);

        for (uint8_t idx = 0; idx < N; idx++)
        {
            m_Materials.emplace_back(MakeShared<Material>());
            m_MaterialProcessed.emplace_back(false);
        }
        m_NumMaterials = N;
    }

    void AStaticMesh::ProcessMaterial(aiMaterial* Material_, uint32_t MaterialIdx)
    {
        // If this material hasn't been processed yet //
        if (!m_MaterialProcessed[MaterialIdx])
        {
            std::vector<TSharedPtr<RHITexture2D>> materialTextures;

            std::vector<TSharedPtr<RHITexture2D>> diffuseMaps  = LoadMaterialTextures(Material_, RHITextureType::Diffuse);
            std::vector<TSharedPtr<RHITexture2D>> specularMaps = LoadMaterialTextures(Material_, RHITextureType::Specular);
            std::vector<TSharedPtr<RHITexture2D>> normalMaps   = LoadMaterialTextures(Material_, RHITextureType::Normal);
            
            materialTextures.insert(materialTextures.end(),     std::make_move_iterator(diffuseMaps.begin()),   std::make_move_iterator(diffuseMaps.end()));
            materialTextures.insert(materialTextures.end(),     std::make_move_iterator(specularMaps.begin()),  std::make_move_iterator(specularMaps.end()));
            materialTextures.insert(materialTextures.end(),     std::make_move_iterator(normalMaps.begin()),    std::make_move_iterator(normalMaps.end()));

            if (materialTextures.size() > 0)
            {
                for (auto& texture : materialTextures)
                {
                    m_Materials[MaterialIdx]->SetTextureByType(texture->GetType(), texture);
                }
                m_MaterialProcessed[MaterialIdx] = true;
            }
        }
    }
    
    std::vector<TSharedPtr<RHITexture2D>> AStaticMesh::LoadMaterialTextures(aiMaterial* Material, RHITextureType Type)
    {
        std::vector<TSharedPtr<RHITexture2D>> textures;
        
        for (uint8_t i = 0; i < Material->GetTextureCount(RHITextureTypeToAssimpTextureType(Type)); i++)
        {
            aiString texFileName;
            Material->GetTexture(RHITextureTypeToAssimpTextureType(Type), i, &texFileName);
            
            std::string texturePath = m_Directory + "/";
            texturePath.append(texFileName.C_Str());
            
            auto& textureManager = Renderer::Get()->GetTexture2DManager();
            
            textureManager.LoadResource(texturePath, Type);
            
            auto& loadedTexture = textureManager.GetResource(texturePath);
            loadedTexture->SetType(Type);

            textures.push_back(textureManager.GetResource(texturePath));
        }
        return textures;
    }

    void AStaticMesh::Draw(void) const
    {
        if(m_bIsModelReadyToDraw)
        {
            const glm::mat4 modelMat = GetModelMat();

            for (auto& mesh : m_SubMeshes)
            {
                mesh->Draw(modelMat);
            }
        }
    }

    void AStaticMesh::OnTick(double DeltaTime)
    {
        if (m_bIsModelImported && !m_bWasModelLoadedOnPrevFrame)
        {
            for (int8_t idx = 0; idx < m_SubMeshes.size(); idx++)
            {
                if (m_SubMeshes[idx]->IsMeshLazyEvaluated() && !m_SubMeshes[idx]->IsManualEvaluationPerformed())
                {
                    {
                        PROFILE_SCOPE("EvaluateMesh");
                        m_SubMeshes[idx]->EvaluateMesh();
                    }
                    {
                        PROFILE_SCOPE("ProcessMaterials");
                        const aiScene* scene = m_ModelImporter->GetScene();
                        ProcessMaterial(scene->mMaterials[m_SubMeshes[idx]->GetMaterialIndex()], m_SubMeshes[idx]->GetMaterialIndex());
                    }
                }
            }
            OnAsyncLoadingFinishedDelegate.ExecuteIfBound(this);
            SceneDelegates::Get()->OnStaticMeshLoaded.Broadcast(this);

            m_bIsModelReadyToDraw = true;
        }
        
        m_bWasModelLoadedOnPrevFrame = m_bIsModelImported;
    }

    void AStaticMesh::OnConstruct(void)
    {
        if(m_bScheduleModelLoadOnConstruct)
        {
            m_LoadModelFuture = std::async(std::launch::async, std::bind(&AStaticMesh::ImportModel,this,std::placeholders::_1), m_ModelFilePath);
        }
        else
        {
            for (auto& mesh : m_SubMeshes)
            {
                mesh->SetStaticMeshOwner(shared_from_this());
            }
        }
    }

    TSharedPtr<AStaticMesh> AStaticMesh::Clone(void)
    {
        std::vector<TSharedPtr<Mesh>> meshes = m_SubMeshes;

        TSharedPtr<AStaticMesh> clone = NewActor<AStaticMesh>(MoveTemp(meshes),GetLocation());

        clone->m_Materials = m_Materials;
        clone->m_NumMaterials = m_NumMaterials;

        clone->m_Directory = m_Directory;
        clone->m_ModelFilePath = m_ModelFilePath;
   
        return clone;
    }

    size_t AStaticMesh::GetNumMaterials(void) const
    {
        return m_Materials.size();
    }

    Material* AStaticMesh::GetMaterialInSlot(uint8_t SlotIndex)
    {
        if (SlotIndex > m_Materials.size() - 1)
        {
            LOG(Materials, Warning, "Bad index. Tried to get material from slot of index {0}. Available slots for this static mesh : [0-{1}]",
                SlotIndex, m_Materials.size()-1);
            return nullptr;
        }
        else
        {
            return m_Materials[SlotIndex].get();
        }
    }

    void AStaticMesh::SetMaterialForSlot(uint8_t SlotIndex, TSharedPtr<Material> Material)
    {
        if (SlotIndex > m_Materials.size() - 1 || m_Materials.size() == 0)
        {
            LOG(Materials, Warning, "Bad index. Tried to set new material in slot of index {0}. Available slots for this static mesh : [0-{1}]",
                SlotIndex, m_Materials.size()-1);
            return;
        }

        m_Materials[SlotIndex] = Material;
    }

    MeshStatistics AStaticMesh::GetMeshStatistics(void) const
    {
        MeshStatistics meshStats;

        for (auto const& mesh : m_SubMeshes)
        {
            meshStats.TrisCount += mesh->GetMeshStatistics().TrisCount;
        }

        return meshStats;
    }
}