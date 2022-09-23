#include "EnginePCH.hpp"

#include "StaticMesh.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Materials/Material.hpp"
#include "Scene/Entities/CoreActor.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssimpHelpers.hpp"


namespace Engine
{
    AStaticMesh::AStaticMesh(std::string const& FilePath, glm::vec3 const& WorldLocation)
        :   Actor(WorldLocation),
            m_ModelFilePath(FilePath)
    {
        m_bScheduleModelLoadOnConstruct = true;
    }

    AStaticMesh::AStaticMesh(std::vector<std::shared_ptr<Mesh>>&& SubMeshes, glm::vec3 const& WorldLocation)
        :   Actor(WorldLocation)
    {
        m_SubMeshes = std::move(SubMeshes);
    }

    void AStaticMesh::LoadModel(std::string_view FilePath)
    {
        Assimp::Importer assimpImporter;
    
        const aiScene* scene = assimpImporter.ReadFile( FilePath.data(),
            aiProcess_Triangulate 
            | aiProcess_FlipUVs 
            | aiProcess_OptimizeMeshes 
            | aiProcess_RemoveRedundantMaterials
            | aiProcess_PreTransformVertices);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG(Assimp, Error, "{0}", assimpImporter.GetErrorString());
            return;
        }
        m_Directory = FilePath.substr(0, FilePath.find_last_of('/'));
        
        InitializeMaterialSlots(scene);
        ProcessNode(scene->mRootNode, scene);
    }
    
    void AStaticMesh::ProcessNode(aiNode* Node, const aiScene* Scene)
    {
        for (uint8_t i = 0; i < Node->mNumMeshes; i++)
        {    
            aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];

            std::shared_ptr<Mesh> subMesh = ProcessMesh(mesh, Scene);
            subMesh->SetStaticMeshOwner(shared_from_this());
            subMesh->GetMeshStatistics().TrisCount = mesh->mNumFaces;

            m_SubMeshes.emplace_back(std::move(subMesh));
        }
    
        for (uint8_t i = 0; i < Node->mNumChildren; i++)
        {
            ProcessNode(Node->mChildren[i], Scene);
        }
    }
    
    std::shared_ptr<Mesh> AStaticMesh::ProcessMesh(aiMesh* _Mesh, const aiScene* Scene)
    {
        std::vector<RHIVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<std::shared_ptr<RHITexture2D>> textures;
    
        /** Process vertices */
        for (unsigned int i = 0; i < _Mesh->mNumVertices; i++)
        {
            RHIVertex vertex;
    
            aiVector3D const& vertexPosition = _Mesh->mVertices[i];
            vertex.Position = { vertexPosition.x, vertexPosition.y, vertexPosition.z };
            
            aiVector3D const& normal = _Mesh->mNormals[i];
            vertex.Normal = { normal.x, normal.y, normal.z };
    
            /** Check if the mesh contain texture coordinates */
            if (_Mesh->mTextureCoords[0])
            {
                aiVector3D const& textureCoords = _Mesh->mTextureCoords[0][i];
                vertex.TexUV = { textureCoords.x, textureCoords.y };
            }
            else
            {
                vertex.TexUV = { 0.f, 0.f };
            }
            vertices.emplace_back(std::move(vertex));
        }
    
        /** Process indices */
        for (uint32_t i = 0; i < _Mesh->mNumFaces; i++)
        {
            aiFace const& face = _Mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        
        std::shared_ptr<Mesh> subMesh = std::make_shared<Mesh>( std::move(vertices),
                                                                std::move(indices));

        /** Process materials */
        if (_Mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = Scene->mMaterials[_Mesh->mMaterialIndex];
            ProcessMaterial(material,_Mesh->mMaterialIndex);
            subMesh->SetMaterialIndex(_Mesh->mMaterialIndex);
        }

        return subMesh;
    }

    void AStaticMesh::InitializeMaterialSlots(const aiScene* Scene)
    {
        m_Materials.clear();
        m_Materials.reserve(Scene->mNumMaterials);

        for (uint8_t idx = 0; idx < Scene->mNumMaterials; idx++)
        {
            m_Materials.emplace_back(nullptr);
        }

        m_NumMaterials = Scene->mNumMaterials;
    }

    void AStaticMesh::ProcessMaterial(aiMaterial* Material_, uint32_t MaterialIdx)
    {
        /* If this material hasn't been processed yet */
        if (m_Materials[MaterialIdx] == nullptr)
        {
            std::vector<std::shared_ptr<RHITexture2D>> diffuseMaps  = LoadMaterialTextures(Material_, RHITextureType::Diffuse);
            std::vector<std::shared_ptr<RHITexture2D>> specularMaps = LoadMaterialTextures(Material_, RHITextureType::Specular);
            
            std::vector<std::shared_ptr<RHITexture2D>> materialTextures;

            materialTextures.insert(materialTextures.end(),
                                    std::make_move_iterator(diffuseMaps.begin()),
                                    std::make_move_iterator(diffuseMaps.end()));

            materialTextures.insert(materialTextures.end(),
                                    std::make_move_iterator(specularMaps.begin()),
                                    std::make_move_iterator(specularMaps.end()));

            if (materialTextures.size() > 0)
            {
                m_Materials[MaterialIdx] = std::make_shared<Material>();

                for (auto& texture : materialTextures)
                {
                    if (texture->GetType() == RHITextureType::Diffuse)
                    {
                        m_Materials[MaterialIdx]->SetDiffuseTexture(texture);
                    }
                    else if (texture->GetType() == RHITextureType::Specular)
                    {

                    }
                }
            }
        }
    }
    
    std::vector<std::shared_ptr<RHITexture2D>> AStaticMesh::LoadMaterialTextures(aiMaterial* Material, RHITextureType Type)
    {
        std::vector<std::shared_ptr<RHITexture2D>> textures;
    
        for (uint8_t i = 0; i < Material->GetTextureCount(RHITextureTypeToAssimpTextureType(Type)); i++)
        {
            aiString texFileName;
            Material->GetTexture(RHITextureTypeToAssimpTextureType(Type), i, &texFileName);
            
            std::string texturePath = m_Directory + "/";
            texturePath.append(texFileName.C_Str());
            
            auto& textureManager = Renderer::Get()->GetTexture2DManager();
    
            textureManager.LoadResource(texturePath, Type);
            textures.push_back(textureManager.GetResource(texturePath));
        }
        return textures;
    }

    void AStaticMesh::Draw(void) const
    {
        const glm::mat4 modelMat = GetModelMat();

        for (auto& mesh : m_SubMeshes)
        {
            mesh->Draw(modelMat);
        }
    }

    void AStaticMesh::OnTick(double DeltaTime)
    {}

    void AStaticMesh::OnConstruct(void)
    {
        if(m_bScheduleModelLoadOnConstruct)
        {
            LoadModel(m_ModelFilePath);
        }
    }

    std::shared_ptr<AStaticMesh> AStaticMesh::Clone(void)
    {
        std::vector<std::shared_ptr<Mesh>> meshes = m_SubMeshes;

        std::shared_ptr<AStaticMesh> clone = NewActor<AStaticMesh>(std::move(meshes),GetLocation());

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

    std::optional<std::reference_wrapper<std::shared_ptr<Material>>> AStaticMesh::GetMaterialInSlot(uint8_t SlotIndex)
    {
        if (SlotIndex > m_Materials.size() - 1)
        {
            LOG(Materials, Warning, "Bad index. Tried to get material from slot of index {0}. Available slots for this static mesh : [0-{1}]",
                SlotIndex, m_Materials.size()-1);
            return std::nullopt;
        }
        else
        {
            return std::ref(m_Materials[SlotIndex]);
        }
    }

    void AStaticMesh::SetMaterialForSlot(uint8_t SlotIndex, std::shared_ptr<Material> Material)
    {
        if (SlotIndex > m_Materials.size() - 1)
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