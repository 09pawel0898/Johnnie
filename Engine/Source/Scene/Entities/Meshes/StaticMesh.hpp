#pragma once

#include "Mesh.hpp"

#include "Renderer/RHI/RHITypes.hpp"
#include "Renderer/RHI/Resources/RHIResourceManager.hpp"

#include "Scene/Entities/Primitives/Actor.hpp"
#include "Scene/Entities/Primitives/Tickable.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Engine
{
    namespace RHI
    {
        class RHITexture2D;
        class RHIShader;
    }
    class Material;

	class AStaticMesh : public Actor
	{
    private:
        std::vector<Mesh>   m_SubMeshes;
        std::string         m_Directory;

    private:
        /** Model Loading */
        
        void LoadModel(std::string_view FilePath);

        void ProcessNode(aiNode* Node, const aiScene* Scene);
        Mesh ProcessMesh(aiMesh* _Mesh, const aiScene* Scene);

        std::vector<std::shared_ptr<RHITexture2D>> LoadMaterialTextures(aiMaterial* Material, RHITextureType Type);

    public:
        AStaticMesh(std::string_view FilePath, glm::vec3 const& WorldLocation = glm::vec3(0.f,0.f,0.f));
        AStaticMesh(std::vector<Mesh>&& SubMeshes, glm::vec3 const& WorldLocation = glm::vec3(0.f, 0.f, 0.f));

	public:
        virtual void Draw(void) const override;
		void OnTick(double DeltaTime) override;

    public:
        /** Materials */
        size_t GetNumMaterials(void) const;
        std::optional<std::reference_wrapper<std::shared_ptr<Material>>> GetMaterialByIndex(uint8_t Index);
	};
}