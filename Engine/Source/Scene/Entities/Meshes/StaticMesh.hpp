#pragma once

#include <future>

#include "Mesh.hpp"

#include "Renderer/RHI/RHITypes.hpp"
#include "Renderer/RHI/Resources/RHIResourceManager.hpp"

#include "Scene/Entities/Primitives/Actor.hpp"
#include "Scene/Entities/Primitives/Tickable.hpp"



struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Assimp
{
    class Importer;
}

namespace Engine
{
    namespace RHI
    {
        class RHITexture2D;
        class RHIShader;
    }
    class Material;

	class AStaticMesh : public Actor, public SharedFromThis<AStaticMesh>
	{
    private:
        std::vector<std::shared_ptr<Mesh>> m_SubMeshes;
        std::string m_Directory;
        std::string m_ModelFilePath;
        mutable std::mutex  m_Mutex;
        bool m_bScheduleModelLoadOnConstruct{ false };
        bool m_bIsModelLoaded = false;
        bool m_bIsModelReadyToDraw = false;
        bool m_bWasModelLoadedOnPrevFrame = false;
        std::future<void> m_LoadModelFuture;

    private:
        /** Model Loading */
        
        void LoadModel(std::string_view FilePath);

        void ProcessNode(aiNode* Node, const aiScene* Scene);
        std::shared_ptr<Mesh> ProcessMesh(aiMesh* _Mesh, const aiScene* Scene);
        void ProcessMaterial(aiMaterial* Material_, uint32_t MaterialIdx);

        std::vector<std::shared_ptr<RHITexture2D>> LoadMaterialTextures(aiMaterial* Material, RHITextureType Type);

    public:
        AStaticMesh(std::string const& FilePath, glm::vec3 const& WorldLocation = glm::vec3(0.f,0.f,0.f));
        AStaticMesh(std::vector<std::shared_ptr<Mesh>>&& SubMeshes, glm::vec3 const& WorldLocation = glm::vec3(0.f, 0.f, 0.f));
        ~AStaticMesh();
	
    public:
        virtual void Draw(void) const override;
		void OnTick(double DeltaTime) override;
        void OnConstruct(void) override;
        std::shared_ptr<AStaticMesh> Clone(void);

    public:
        /** Materials */
        std::vector<std::shared_ptr<Material>> m_Materials;
        uint8_t m_NumMaterials;

        size_t GetNumMaterials(void) const;
        std::optional<std::reference_wrapper<std::shared_ptr<Material>>> GetMaterialInSlot(uint8_t SlotIndex);
        
        void SetMaterialForSlot(uint8_t SlotIndex, std::shared_ptr<Material> Material);

        /** Stats */
        MeshStatistics GetMeshStatistics(void) const;

    private:
        void InitializeMaterialSlots(const aiScene* Scene);
    };
}