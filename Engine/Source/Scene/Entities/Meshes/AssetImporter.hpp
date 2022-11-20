#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"
#include "Utilities/Singleton.hpp"
#include "Utilities/Delegates.hpp"
#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <future>

namespace Engine
{
	class ASkeletalMesh;
	class Mesh;
	class SkinnedMesh;
	class Animation;
	struct AnimatedNodeData;

	enum class AssetImporterType : uint8_t
	{
		StaticModel,
		SkeletalModel,
		Animation
	};

	struct ModelView
	{
		uint32_t TotalVertices = 0;
		uint32_t TotalIndices = 0;
		uint32_t TotalBones = 0;

		struct MeshView
		{
			uint32_t Index = Index::None;

			uint32_t Vertices = 0;
			uint32_t Indices = 0;
			uint32_t Bones = 0;
		};
		
		std::vector<MeshView> Meshes;
	};

	class AssetImporter
	{
	protected:
		std::string			m_RootDirectory;
		Assimp::Importer	m_AssimpImporter = Assimp::Importer();
		AssetImporterType	m_ImporterType;
		
	public:
		AssetImporter() = default;

		AssetImporter(AssetImporter const&) = delete;
		AssetImporter(AssetImporter&&) = delete;

		AssetImporter& operator=(AssetImporter const&) = delete;
		AssetImporter& operator=(AssetImporter&&) = delete;

	public:
		Assimp::Importer& GetImporter(void);
		Assimp::Importer const& GetImporter(void) const;
		const aiScene* GetScene(void);
		
		std::string const& GetRootDirectory(void);
	};

	class ModelImporter : public AssetImporter
	{
	protected:
		bool		m_bHasEmbeddedTextures{ false };
		bool		m_bIsModelImported{ false };
		glm::vec3	m_RootScale = glm::vec3(1.f, 1.f, 1.f);
		
		std::future<void>	m_ImportModelFuture;
	
	public:
		glm::vec3 GetRootScale(void) const
		{
			return m_RootScale;
		}

	public:
		virtual void AsyncImportModel(std::string_view FilePath) = 0;

		bool HasEmbededTextures(void);
		bool IsModelAlreadyLoaded(void);
	};

	class StaticModelImporter final : public ModelImporter
	{
	private:

	public:
		StaticModelImporter();

		void AsyncImportModel(std::string_view FilePath) override;
	};

	class SkeletalModelImporter final : public ModelImporter
	{
	private:
		TWeakPtr<ASkeletalMesh>	m_SkeletalMesh;
		
		ModelView				m_ModelView;
		ModelSkeletonData		m_SkeletonData;

		glm::mat4				m_GlobalInverseTransform;

	public:
		explicit SkeletalModelImporter(TWeakPtr<ASkeletalMesh> SkeletalMesh);

		void AsyncImportModel(std::string_view FilePath) override;

	private:
		void AsyncImportModel_Internal(std::string_view FilePath, uint32_t Flags);

		void ParseScene(const aiScene* Scene);


		void PreprocessMeshes(const aiScene* Scene);
		void PreprocessSingleMesh(uint16_t Index, const aiMesh* AiMesh);

		void ParseMeshes(const aiScene* Scene);
		void ParseSingleMesh(uint16_t Index, const aiMesh* AiMesh);
		TSharedPtr<SkinnedMesh> ParseSingleMeshData(uint16_t Index, const aiMesh* AiMesh);

		void ParseMeshBones(uint16_t MeshIndex, const aiMesh* AiMesh);
		void ParseSingleBone(uint16_t MeshIndex, const aiBone* AnimatedBoneData);

		uint32_t GetBoneID(const aiBone* AnimatedBoneData);
		aiNode* GetRootBone(aiNode* SceneRootNode);
		void ProcessNode(aiNode* Node);
		aiNode* m_RootBone = nullptr;

		void ReadNodeHierarchy(float AnimationTimeInTicks, const aiNode* Node, glm::mat4 const& ParentTransformMatrix);

		const aiNodeAnim* FindNodeAnim(const aiAnimation* Animation, std::string const& NodeName);
		uint32_t FindPosition(float AnimationTimeInTicks, const aiNodeAnim* NodeAnim);
		uint32_t FindRotation(float AnimationTimeInTicks, const aiNodeAnim* NodeAnim);
		uint32_t FindScaling(float AnimationTimeInTicks, const aiNodeAnim* NodeAnim);

		void CalculateInterpolatedScaling(aiVector3D& OutScale, float AnimationTimeInTicks, const aiNodeAnim* NodeAnim);
		void CalculateInterpolatedRotation(aiQuaternion& OutQuat, float AnimationTimeInTicks, const aiNodeAnim* NodeAnim);
		void CalculateInterpolatedLocation(aiVector3D& OutLocation, float AnimationTimeInTicks, const aiNodeAnim* NodeAnim);

		void InitializeRequiredNodes(const aiNode* Root);

	public:
		void GetBoneTransforms(float AnimationTimeInSeconds, std::vector<glm::mat4>& OutTransformMatrices);
		void MarkRequiredNodesForBone(const aiBone* AnimatedBoneData);
	};

	aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHIMapTextureType RHIMapTextureType);

	DECLARE_MULTICAST_DELEGATE(OnAnimationsAsyncLoadingFinishedDelegate, std::vector<TUniquePtr<Animation>>&&);
	DECLARE_MULTICAST_DELEGATE(OnAnimationAsyncLoadingFinishedDelegate, TUniquePtr<Animation>&&);

	class AnimationImporter : public AssetImporter
	{
	private:
		std::mutex m_LoadedAnimationsLock;
		std::vector<TUniquePtr<Animation>>	m_LoadedAnimations;

		std::variant<	OnAnimationAsyncLoadingFinishedDelegate, 
						OnAnimationsAsyncLoadingFinishedDelegate> m_OnAsyncLoadingFinished;

		std::future<void> m_ReadSceneFuture;
		std::vector< std::future<void>> m_ImportAnimationFutures;

		
	public:
		AnimationImporter() = default;

		void AsyncImportFirstAnimation(std::string_view FilePath, OnAnimationAsyncLoadingFinishedDelegate OnAsyncLoadingFinished = OnAnimationAsyncLoadingFinishedDelegate());
		void AsyncImportAllAnimations(std::string_view FilePath, OnAnimationsAsyncLoadingFinishedDelegate OnAsyncLoadingFinished = OnAnimationsAsyncLoadingFinishedDelegate());

	private:
		void AsyncImportScene_Internal(std::string_view FilePath, bool ImportAllAnimations);
		void AsyncImportAnimation_Internal(const aiAnimation* AiAnimation);

		void ReadBonesData(Animation* Anim, const aiAnimation* AiAnimation);
		void ReadHeirarchyData(AnimatedNodeData* AnimatedNode, const aiNode* Node);

		void NotifyLoadingFinished(void);
	};
}
