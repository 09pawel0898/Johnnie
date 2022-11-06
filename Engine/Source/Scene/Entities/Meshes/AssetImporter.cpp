#include "EnginePCH.hpp"

#include "AssetImporter.hpp"
#include "Log/Log.hpp"


namespace Engine
{
	Assimp::Importer& Engine::AssetImporter::GetImporter(void)
	{
		return m_AssimpImporter;
	}

	Assimp::Importer const& Engine::AssetImporter::GetImporter(void) const
	{
		return m_AssimpImporter;
	}

	const aiScene* Engine::AssetImporter::GetScene(void)
	{
		return m_AssimpImporter.GetScene();
	}

	void AssetImporter::AsyncImportModel_Internal(std::string_view FilePath, uint32_t Flags)
	{
		const aiScene* Scene = m_AssimpImporter.ReadFile(FilePath.data(), Flags);

		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
		{
			LOG(Assimp, Error, "{0}", m_AssimpImporter.GetErrorString());
			return;
		}
		//m_Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
		OnMaterialsInforAcquiredDelegate.ExecuteIfBound(Scene->mNumMaterials);
		ParseScene(Scene);
	}

	void AssetImporter::ParseScene(const aiScene* Scene)
	{
		ParseMeshes(Scene);
	}

	void AssetImporter::ParseMeshes(const aiScene* Scene)
	{
		uint32_t TotalVertices = 0;
		uint32_t TotalIndices = 0;
		uint32_t TotalBones = 0;

		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			ParseSingleMesh(Scene->mMeshes[idx], TotalVertices, TotalIndices, TotalBones);
		}
	}

	void AssetImporter::ParseSingleMesh(const aiMesh* Mesh, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount)
	{
		TotalVerticesCount += Mesh->mNumVertices;
		TotalIndicesCount += Mesh->mNumFaces * 3;
		TotalBonesCount += Mesh->mNumBones;

		if (m_ImporterType == AssetImporterType::SkeletalModel && Mesh->HasBones())
		{
			ParseMeshBones(Mesh);
		}
	}

	void AssetImporter::ParseMeshBones(const aiMesh* Mesh)
	{
		for (uint16_t idx = 0; idx < Mesh->mNumBones; idx++)
		{
			ParseSingleBone(idx, Mesh->mBones[idx]);
		}
	}

	void AssetImporter::ParseSingleBone(uint16_t BoneIndex, const aiBone* Bone)
	{
		for (uint16_t idx = 0; idx < Bone->mNumWeights; idx++)
		{
			const aiVertexWeight& VertexWeight = Bone->mWeights[idx];
			LOG(Assimp, Trace, "{0}x{1}", VertexWeight.mVertexId, VertexWeight.mWeight);
		}
	}

	aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHITextureType RHITextureType)
	{
		static std::unordered_map<RHI::RHITextureType, aiTextureType> map =
		{
			{RHI::RHITextureType::Diffuse,	aiTextureType_DIFFUSE	},
			{RHI::RHITextureType::Specular,	aiTextureType_SPECULAR	},
			{RHI::RHITextureType::Normal,	aiTextureType_NORMALS	},
		};
		return map[RHITextureType];
	}

	StaticModelImporter::StaticModelImporter()
	{
		m_ImporterType = AssetImporterType::StaticModel;
	}

	void StaticModelImporter::AsyncImportModel(std::string_view FilePath)
	{
	}

	SkeletalModelImporter::SkeletalModelImporter()
	{
		m_ImporterType = AssetImporterType::SkeletalModel;
	}

	void SkeletalModelImporter::AsyncImportModel(std::string_view FilePath)
	{
		uint32_t Flags =  aiProcess_Triangulate
						| aiProcess_RemoveRedundantMaterials
						| aiProcess_OptimizeMeshes
						| aiProcess_GenNormals
						| aiProcess_JoinIdenticalVertices
						| aiProcess_CalcTangentSpace;

		m_ImportModelFuture = std::async(	std::launch::async, 
											std::bind(&AssetImporter::AsyncImportModel_Internal, this, std::placeholders::_1,std::placeholders::_2), 
											FilePath,
											Flags);
	}

	void VertexBoneData::AddBoneData(uint32_t BoneID, float Weight)
	{
		for (uint16_t idx = 0; idx < s_MaxBonesPerVertex; idx++)
		{
			if (Weights[idx] == 0.f)
			{
				BoneIDs[idx] = BoneID;
				Weights[idx] = Weight;
				return;
			}
		}
		Check(false);
	}
}
