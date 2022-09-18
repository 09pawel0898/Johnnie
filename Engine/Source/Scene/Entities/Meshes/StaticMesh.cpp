#include "EnginePCH.hpp"

#include "StaticMesh.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/Renderer.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssimpHelpers.hpp"


namespace Engine
{
    AStaticMesh::AStaticMesh(std::string_view FilePath, glm::vec3 const& WorldLocation)
        :   Actor(WorldLocation)
    {
        LoadModel(FilePath);
    }

    AStaticMesh::AStaticMesh(std::vector<Mesh>&& SubMeshes, glm::vec3 const& WorldLocation)
        :   Actor(WorldLocation)
    {
        m_SubMeshes = std::move(SubMeshes);
    }

    void AStaticMesh::LoadModel(std::string_view FilePath)
    {
        Assimp::Importer assimpImporter;
    
        const aiScene* scene = assimpImporter.ReadFile( FilePath.data(),
                                                        aiProcess_Triangulate | aiProcess_FlipUVs);
    
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG(Assimp, Error, "{0}", assimpImporter.GetErrorString());
            return;
        }
        m_Directory = FilePath.substr(0, FilePath.find_last_of('/'));
    
        ProcessNode(scene->mRootNode, scene);
    }
    
    void AStaticMesh::ProcessNode(aiNode* Node, const aiScene* Scene)
    {
        for (uint8_t i = 0; i < Node->mNumMeshes; i++)
        {
            aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
            m_SubMeshes.emplace_back(ProcessMesh(mesh, Scene));
        }
    
        for (uint8_t i = 0; i < Node->mNumChildren; i++)
        {
            ProcessNode(Node->mChildren[i], Scene);
        }
    }
    
    Mesh AStaticMesh::ProcessMesh(aiMesh* _Mesh, const aiScene* Scene)
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
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
    
        /** Process materials */
        if (_Mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = Scene->mMaterials[_Mesh->mMaterialIndex];
    
            std::vector<std::shared_ptr<RHITexture2D>> diffuseMaps  = LoadMaterialTextures(material, RHITextureType::Diffuse);
            std::vector<std::shared_ptr<RHITexture2D>> specularMaps = LoadMaterialTextures(material, RHITextureType::Specular);
    
            textures.insert(textures.end(), 
                            std::make_move_iterator(diffuseMaps.begin()), 
                            std::make_move_iterator(diffuseMaps.end()));
    
            textures.insert(textures.end(), 
                            std::make_move_iterator(specularMaps.begin()), 
                            std::make_move_iterator(specularMaps.end()));
        }
    
        return Mesh(std::move(vertices),
                    std::move(indices), 
                    std::move(textures));
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
        auto& shaderManager = Renderer::Get()->GetShaderManager();
        auto& staticMeshShader = shaderManager.GetResource("Shader_StaticMesh");
        
        const glm::mat4 modelMat = GetModelMat();

        for (auto& mesh : m_SubMeshes)
        {
            mesh.Draw(staticMeshShader, modelMat);
        }
    }

    void AStaticMesh::OnTick(double DeltaTime)
    {
    }
}