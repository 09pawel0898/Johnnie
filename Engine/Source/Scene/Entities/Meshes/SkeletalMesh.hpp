#pragma once

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
    private:
        TUniquePtr<AssetImporter> m_ModelImporter{ nullptr };
        
        OnSkeletalMeshAsyncLoadingFinishedDelegate m_OnAsyncLoadingFinishedDelegate;
        std::string m_Directory;
        std::string m_ModelFilePath;

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

        void InitializeMaterialSlots(uint8_t NumMaterials);
        void ImportModel();
        void Emplace_N_MaterialSlots(uint8_t N);
    };
}