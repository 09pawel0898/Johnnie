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
		bool m_bIsBusy{ false };

	public:
		AssetImporter() = default;

		AssetImporter(AssetImporter const&) = delete;
		AssetImporter(AssetImporter&&) = delete;

		AssetImporter& operator=(AssetImporter const&) = delete;
		AssetImporter& operator=(AssetImporter&&) = delete;

	public:
		Assimp::Importer& GetImporter(void);
		Assimp::Importer const& GetImporter(void) const;
		const aiScene* GetScene(void) const;
		bool IsBusy(void) const;
		std::string const& GetRootDirectory(void);
	};

	FORCEINLINE bool AssetImporter::IsBusy(void) const
	{
		return m_bIsBusy;
	}

	class ModelImporter : public AssetImporter
	{
	protected:
		bool m_bHasEmbeddedTextures{ false };
		bool m_bIsModelImported{ false };
		
		std::future<void> m_ImportModelFuture;
	
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
		SkinnedMeshData			m_SkinnedMeshData;
		SkeletonData			m_SkeletonData;

		glm::mat4				m_GlobalInverseTransform;

	public:
		explicit SkeletalModelImporter(TWeakPtr<ASkeletalMesh> SkeletalMesh);

		void AsyncImportModel(std::string_view FilePath) override;
		SkeletonData& GetSkeletonData(void) { return m_SkeletonData; };
		SkeletonData const& GetSkeletonData(void) const { return m_SkeletonData; };

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

		void ParseHierarchyData(const aiNode* AiRootNode);
		void ParseHierarchyData_BuildHierarchy(NodeData* Node, const aiNode* AiNode);
		void ParseHierarchyData_FixParenting(NodeData* Node);


		void PrintSkeleton(aiNode* Node, uint16_t Deep = 0);

		uint32_t GetBoneID(const aiBone* AnimatedBoneData);
		
	public:
		static void FindRootBone(NodeData* Node, NodeData** OutResult, std::map<std::string,uint32_t> const& BoneNameIndexMap);

	public:
		void MarkRequiredNodesForBone(const aiBone* AiBone);
		glm::mat4 GetGlobalInverseTransform(void) const
		{
			return m_GlobalInverseTransform;
		}
	};

	aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHIMapTextureType RHIMapTextureType);

	DECLARE_DELEGATE(OnAnimationsAsyncLoadingFinishedDelegate, std::vector<Animation>&);
	DECLARE_DELEGATE(OnAnimationAsyncLoadingFinishedDelegate, Animation&);

	class FixedSkeletonData
	{
		std::map<std::string, uint32_t>	BoneNameIndexMap;
		std::vector<BoneData>			BonesData;
		std::map<std::string, NodeData>	RequiredNodes;
	};

	class AnimationImporter : public AssetImporter
	{
	private:
		std::mutex m_LoadedAnimationsLock;
		std::vector<Animation> m_LoadedAnimations;

		std::variant<	OnAnimationAsyncLoadingFinishedDelegate, 
						OnAnimationsAsyncLoadingFinishedDelegate> m_OnAsyncLoadingFinished;

		std::future<void> m_ReadSceneFuture;
		std::vector< std::future<void>> m_ImportAnimationFutures;

	public:
		TWeakPtr<ASkeletalMesh> m_SkeletalMesh;
		std::map<std::string, uint32_t> m_BoneNameIndexMap;

	public:
		AnimationImporter() = default;

		void AsyncImportFirstAnimation(std::string_view FilePath, OnAnimationAsyncLoadingFinishedDelegate OnAsyncLoadingFinished = OnAnimationAsyncLoadingFinishedDelegate());
		void AsyncImportAllAnimations(std::string_view FilePath, OnAnimationsAsyncLoadingFinishedDelegate OnAsyncLoadingFinished = OnAnimationsAsyncLoadingFinishedDelegate());

	private:
		void AsyncImportScene_Internal(std::string_view FilePath, bool ImportAllAnimations);
		void AsyncImportAnimation_Internal(const aiAnimation* AiAnimation);

		void PrintSkeleton(aiNode* Node, uint16_t Deep =0);

		void ReadBonesData(Animation* Anim, const aiAnimation* AiAnimation);
		void ReadHeirarchyData(AnimatedNodeData* AnimatedNode, const aiNode* Node);

		void NotifyLoadingFinished(void);

		aiNode* GetRootBone(aiNode* SceneRootNode);
		void FindRootBone(aiNode* Node, aiNode** OutResult);
	};
}
