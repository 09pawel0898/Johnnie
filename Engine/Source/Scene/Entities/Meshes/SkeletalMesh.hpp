#pragma once

#include "Scene/Entities/Primitives/Actor.hpp"
#include "Utilities/Delegates.hpp"
#include "Renderer/Materials/Material.hpp"

#include <future>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
class aiBone;

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
        std::future<void> m_LoadModelFuture;
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

        void InitializeMaterialSlots(void);
        void ImportModel(std::string_view FilePath);
        void Emplace_N_MaterialSlots(uint8_t N);

    private:
        void ParseScene(const aiScene* Scene);
        void ParseMeshes(const aiScene* Scene);
        void ParseSingleMesh(const aiMesh* Mesh, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount);
        void ParseMeshBones(const aiMesh* Mesh);
        void ParseSingleBone(uint16_t BoneIndex, const aiBone* Bone);
    };
}