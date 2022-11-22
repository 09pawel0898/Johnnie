#pragma once

#include "../Mesh.hpp"
#include "Scene/Entities/Primitives/Actor.hpp"
#include "Utilities/Delegates.hpp"
#include "Renderer/Materials/Material.hpp"
#include "../MaterialHelpers.hpp"

#include <future>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
struct aiBone;

namespace Engine
{
    class ModelImporter;
    class ASkeletalMesh;

    DECLARE_DELEGATE(OnSkeletalMeshAsyncLoadingFinishedDelegate, ASkeletalMesh*);


    class ASkeletalMesh : public Actor, public SharedFromThis<ASkeletalMesh>
    {
        friend class SkeletalModelImporter;

    private:
        std::string m_ModelFilePath;

        TSharedPtr<ModelImporter>                   m_ModelImporter{ nullptr };
        OnSkeletalMeshAsyncLoadingFinishedDelegate  m_OnAsyncLoadingFinishedDelegate;
        
        std::vector<TSharedPtr<SkinnedMesh>>        m_SubMeshes;
        MaterialsContainer                          m_MaterialsContainer;

        std::vector<glm::mat4>                      m_CurrentBoneTransformations;

        bool m_bIsModelReadyToDraw{ false };
        bool m_bIsModelImported{ false };

    public:
        ASkeletalMesh(std::string const& FilePath, OnSkeletalMeshAsyncLoadingFinishedDelegate OnLoadingFinished = OnSkeletalMeshAsyncLoadingFinishedDelegate(), glm::vec3 const& WorldLocation = glm::vec3(0.f, 0.f, 0.f));
    
        ~ASkeletalMesh();

    public:
        virtual void Draw(void) override;
        
        virtual void OnTick(double DeltaTime) override;

        virtual void OnConstruct(void) override;
        
        virtual glm::mat4 GetModelMat(void) const override;

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
        TSharedPtr<ModelImporter> const& GetImporter(void) const;

        void SetCurrentBoneTransformations(std::vector<glm::mat4> const& BoneTransformations);
        std::vector<glm::mat4> const& GetCurrentBoneTransformations(void) const;

        bool IsActorReadyToDraw(void) const;
    };

    FORCEINLINE void ASkeletalMesh::SetCurrentBoneTransformations(std::vector<glm::mat4> const& BoneTransformations)
    {
        m_CurrentBoneTransformations = BoneTransformations;
    }

    FORCEINLINE std::vector<glm::mat4> const& ASkeletalMesh::GetCurrentBoneTransformations(void) const
    {
        return m_CurrentBoneTransformations;
    }

    FORCEINLINE bool ASkeletalMesh::IsActorReadyToDraw(void) const
    {
        return m_bIsModelReadyToDraw;
    }
}