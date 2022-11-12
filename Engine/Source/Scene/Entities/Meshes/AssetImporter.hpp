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

	enum class AssetImporterType : uint8_t
	{
		StaticModel,
		SkeletalModel
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

		Assimp::Importer	m_AssimpImporter;
		std::future<void>	m_ImportModelFuture;

		AssetImporterType	m_ImporterType;
		bool				m_bHasEmbeddedTextures{ false };
		bool				m_bIsModelImported{ false };

	public:
		AssetImporter() = default;

	public:
		Assimp::Importer& GetImporter(void);
		Assimp::Importer const& GetImporter(void) const;
		const aiScene* GetScene(void);

	public:
		virtual void AsyncImportModel(std::string_view FilePath) = 0;

		bool HasEmbededTextures(void);
		bool IsModelAlreadyLoaded(void);
		std::string const& GetRootDirectory(void);
	};

	class StaticModelImporter final : public AssetImporter
	{
	private:

	public:
		StaticModelImporter();

		void AsyncImportModel(std::string_view FilePath) override;
	};

	class SkeletalModelImporter final : public AssetImporter
	{
	private:
		TWeakPtr<ASkeletalMesh>	m_SkeletalMesh;
		
		ModelView				m_ModelView;
		SkeletonData			m_SkeletonData;

	public:
		SkeletalModelImporter(TWeakPtr<ASkeletalMesh> SkeletalMesh);

		void AsyncImportModel(std::string_view FilePath) override;

	private:
		void AsyncImportModel_Internal(std::string_view FilePath, uint32_t Flags);

		void ParseScene(const aiScene* Scene);
		void ParseMeshes(const aiScene* Scene);
		void PreprocessMeshes(const aiScene* Scene);
		void PreprocessSingleMesh(uint16_t Index, const aiMesh* AiMesh);
		void ParseSingleMesh(uint16_t Index, const aiMesh* AiMesh);
		TSharedPtr<SkinnedMesh> ParseSingleMeshData(uint16_t Index, const aiMesh* AiMesh);
		void ParseMeshBones(uint16_t MeshIndex, const aiMesh* AiMesh);
		void ParseSingleBone(uint16_t MeshIndex, const aiBone* Bone);
		uint32_t GetBoneID(const aiBone* Bone);
	
	};


	aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHIMapTextureType RHIMapTextureType);
}

