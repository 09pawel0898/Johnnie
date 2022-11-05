#pragma once

#include "Scene/Entities/Primitives/Actor.hpp"
#include "Utilities/Delegates.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Engine
{
    class AssetImporter;
    class ASkeletalMesh;

    DECLARE_DELEGATE(OnSkeletalMeshAsyncLoadingFinishedDelegate, ASkeletalMesh*);

    class ASkeletalMesh : public Actor, public SharedFromThis<ASkeletalMesh>
    {
    private:
        TUniquePtr<AssetImporter> m_ModelImporter{ nullptr };
        
        OnSkeletalMeshAsyncLoadingFinishedDelegate OnAsyncLoadingFinishedDelegate;

    public:
        ASkeletalMesh(std::string const& FilePath, OnSkeletalMeshAsyncLoadingFinishedDelegate OnLoadingFinished = OnSkeletalMeshAsyncLoadingFinishedDelegate(), glm::vec3 const& WorldLocation = glm::vec3(0.f, 0.f, 0.f));
    
        ~ASkeletalMesh();

    public:
        virtual void Draw(void) const override;
        
        void OnTick(double DeltaTime) override;

        void OnConstruct(void) override;
    };
}