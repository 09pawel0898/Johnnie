#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"
#include "Utilities/Singleton.hpp"
#include "Utilities/Delegates.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <future>

namespace Engine
{
	enum class AssetImporterType : uint8_t
	{
		StaticModel,
		SkeletalModel
	};

	struct VertexBoneData
	{
	private:
		static constexpr inline int8_t s_MaxBonesPerVertex = 10;

		std::array<uint32_t, s_MaxBonesPerVertex> BoneIDs;
		std::array<float, s_MaxBonesPerVertex> Weights;

	public:
		VertexBoneData() = default;

		void AddBoneData(uint32_t VertID, uint32_t BoneID, float Weight);
	};

	struct SkeletonData
	{
		std::vector<VertexBoneData> VertexToBones;
		std::vector<uint32_t> MeshBaseVertex;
		std::map<std::string, uint32_t> BoneNameIndexMap;
	};

	DECLARE_DELEGATE(OnMaterialsInfoAcquired, uint8_t);

	class AssetImporter
	{
	protected:
		Assimp::Importer m_AssimpImporter;
		std::future<void> m_ImportModelFuture;
		AssetImporterType m_ImporterType;


	public:
		AssetImporter() = default;

	public:
		Assimp::Importer& GetImporter(void);
		Assimp::Importer const& GetImporter(void) const;
		const aiScene* GetScene(void);

	public:
		OnMaterialsInfoAcquired OnMaterialsInforAcquiredDelegate;

		virtual void AsyncImportModel(std::string_view FilePath) = 0;
	};

	class StaticModelImporter : public AssetImporter
	{
	private:

	public:
		StaticModelImporter();

		void AsyncImportModel(std::string_view FilePath) override;
	};
	
	class SkeletalModelImporter : public AssetImporter
	{
	private:
		SkeletonData m_SkeletonData;

	public:
		SkeletalModelImporter();

		void AsyncImportModel(std::string_view FilePath) override;

	private:
		void AsyncImportModel_Internal(std::string_view FilePath, uint32_t Flags);

		void ParseScene(const aiScene* Scene);
		void ParseMeshes(const aiScene* Scene);
		void PreprocessMeshes(const aiScene* Scene, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount);
		void PreprocessSingleMesh(uint16_t Index, const aiMesh* Mesh, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount);
		void ParseSingleMesh(uint16_t Index, const aiMesh* Mesh, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount);
		void ParseMeshBones(uint16_t MeshIndex, const aiMesh* Mesh);
		void ParseSingleBone(uint16_t MeshIndex, const aiBone* Bone);
		uint32_t GetBoneID(const aiBone* Bone);
	};


	aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHITextureType RHITextureType);
}

