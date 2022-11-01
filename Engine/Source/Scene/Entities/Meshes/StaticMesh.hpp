#pragma once

#include <future>

#include "Mesh.hpp"

#include "Renderer/RHI/RHITypes.hpp"
#include "Renderer/RHI/Resources/RHIResourceManager.hpp"
#include "Renderer/Materials/Material.hpp"

#include "Scene/Entities/Primitives/Actor.hpp"
#include "Scene/Entities/Primitives/Tickable.hpp"

#include "Utilities/Delegates.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Engine
{
    class AssetImporter;

    namespace RHI
    {
        class RHITexture2D;
        class RHIShader;
    }

    DECLARE_DELEGATE(OnStaticMeshAsyncLoadingFinishedDelegate, AStaticMesh*);

	class AStaticMesh : public Actor, public SharedFromThis<AStaticMesh>
	{
    private:
        TUniquePtr<AssetImporter> m_ModelImporter{ nullptr };

        std::vector<TSharedPtr<Mesh>> m_SubMeshes;

        std::string m_Directory;
        std::string m_ModelFilePath;

        bool m_bIsModelImported = false;
        bool m_bScheduleModelLoadOnConstruct{ false };
        bool m_bIsModelReadyToDraw = false;
        bool m_bWasModelLoadedOnPrevFrame = false;
        std::future<void> m_LoadModelFuture;
        OnStaticMeshAsyncLoadingFinishedDelegate OnAsyncLoadingFinishedDelegate;

    private:
        /** Model Loading */
        
        void ImportModel(std::string_view FilePath);

        void ProcessNode(aiNode* Node, const aiScene* Scene);
        TSharedPtr<Mesh> ProcessMesh(aiMesh* Mesh_, const aiScene* Scene);
        void ProcessMaterial(aiMaterial* Material_, uint32_t MaterialIdx);

        std::vector<TSharedPtr<RHITexture2D>> LoadMaterialTextures(aiMaterial* Material, RHITextureType Type);

    public:
        AStaticMesh(std::string const& FilePath, OnStaticMeshAsyncLoadingFinishedDelegate OnLoadingFinished = OnStaticMeshAsyncLoadingFinishedDelegate(), glm::vec3 const& WorldLocation = glm::vec3(0.f,0.f,0.f));
        AStaticMesh(std::vector<TSharedPtr<Mesh>>&& SubMeshes, glm::vec3 const& WorldLocation = glm::vec3(0.f, 0.f, 0.f));
        
        ~AStaticMesh();
	
    public:
        virtual void Draw(void) const override;
		void OnTick(double DeltaTime) override;
        void OnConstruct(void) override;
        TSharedPtr<AStaticMesh> Clone(void);

        /** Stats */
        MeshStatistics GetMeshStatistics(void) const;
    private:
        /** Materials */
        std::vector<TSharedPtr<Material>> m_Materials;
        std::vector<bool> m_MaterialProcessed;

        uint8_t m_NumMaterials;

        void InitializeMaterialSlots(MaterialsEvaluateMethod EvaluateMethod);
        void Emplace_N_MaterialSlots(uint8_t N);

    public:
        void SetMaterialForSlot(uint8_t SlotIndex, TSharedPtr<Material> Material);
        Material* GetMaterialInSlot(uint8_t SlotIndex);
       
        size_t GetNumMaterials(void) const;
    };
}