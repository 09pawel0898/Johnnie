#pragma once

#include "Mesh.hpp"
#include "Scene/Entities/Primitives/Actor.hpp"
#include "Utilities/Delegates.hpp"
#include "Renderer/Materials/Material.hpp"

#include <future>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
struct aiBone;

namespace Engine
{
    class AssetImporter;
    class ASkeletalMesh;

    DECLARE_DELEGATE(OnSkeletalMeshAsyncLoadingFinishedDelegate, ASkeletalMesh*);


    class ASkeletalMesh : public Actor, public SharedFromThis<ASkeletalMesh>
    {
        friend class SkeletalModelImporter;

    private:
        TUniquePtr<AssetImporter> m_ModelImporter{ nullptr };
        
        OnSkeletalMeshAsyncLoadingFinishedDelegate m_OnAsyncLoadingFinishedDelegate;
        std::string m_Directory;
        std::string m_ModelFilePath;

        std::vector<TSharedPtr<Mesh>> m_SubMeshes;

    public:
        ASkeletalMesh(std::string const& FilePath, OnSkeletalMeshAsyncLoadingFinishedDelegate OnLoadingFinished = OnSkeletalMeshAsyncLoadingFinishedDelegate(), glm::vec3 const& WorldLocation = glm::vec3(0.f, 0.f, 0.f));
    
        ~ASkeletalMesh();

    public:
        virtual void Draw(void) const override;
        
        void OnTick(double DeltaTime) override;

        void OnConstruct(void) override;

    private:
        std::vector<TSharedPtr<Material>> m_Materials;
        std::vector<bool> m_MaterialProcessed;
        uint8_t m_NumMaterials;
        
        bool m_bIsModelReadyToDraw{ false };
        bool m_bIsModelImported{ false };
        bool m_bWasModelLoadedOnPrevFrame{ false };

        void InitializeMaterialSlots(uint8_t NumMaterials);
        void ImportModel();
        void Emplace_N_MaterialSlots(uint8_t N);

        void ProcessMaterial(aiMaterial* Material_, uint32_t MaterialIdx);
        std::vector<TSharedPtr<RHITexture2D>> LoadMaterialTextures(aiMaterial* Material, RHITextureType Type);

    public:
        Material* GetMaterialInSlot(uint8_t SlotIndex);
        size_t GetNumMaterials(void) const;
        void SetMaterialForSlot(uint8_t SlotIndex, TSharedPtr<Material> Material);
    };
}