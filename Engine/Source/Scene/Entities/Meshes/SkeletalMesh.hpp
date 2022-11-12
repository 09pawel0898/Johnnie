#pragma once

#include "Mesh.hpp"
#include "Scene/Entities/Primitives/Actor.hpp"
#include "Utilities/Delegates.hpp"
#include "Renderer/Materials/Material.hpp"
#include "MaterialHelpers.hpp"

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
        std::string m_ModelFilePath;

        TSharedPtr<AssetImporter>                   m_ModelImporter{ nullptr };
        OnSkeletalMeshAsyncLoadingFinishedDelegate  m_OnAsyncLoadingFinishedDelegate;
        
        std::vector<TSharedPtr<SkinnedMesh>>        m_SubMeshes;
        MaterialsContainer                          m_MaterialsContainer;

        bool m_bIsModelReadyToDraw{ false };
        bool m_bIsModelImported{ false };

    public:
        ASkeletalMesh(std::string const& FilePath, OnSkeletalMeshAsyncLoadingFinishedDelegate OnLoadingFinished = OnSkeletalMeshAsyncLoadingFinishedDelegate(), glm::vec3 const& WorldLocation = glm::vec3(0.f, 0.f, 0.f));
    
        ~ASkeletalMesh();

    public:
        virtual void Draw(void) const override;
        
        void OnTick(double DeltaTime) override;

        void OnConstruct(void) override;

    private:
        void ImportModel(void);
        void InitializeMaterialSlots(uint8_t NumMaterials);
        void EvaluateMeshes(void);

    public:
        Material* GetMaterialInSlot(uint8_t SlotIndex);
        size_t GetNumMaterials(void) const;
        void SetMaterialForSlot(uint8_t SlotIndex, TSharedPtr<Material> const& Material);

    public:
        std::string const& GetFilePath(void) const;
    };
}