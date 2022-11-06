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
											std::bind(&SkeletalModelImporter::AsyncImportModel_Internal, this, std::placeholders::_1,std::placeholders::_2),
											FilePath,
											Flags);
	}

	void SkeletalModelImporter::AsyncImportModel_Internal(std::string_view FilePath, uint32_t Flags)
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

	void SkeletalModelImporter::ParseScene(const aiScene* Scene)
	{
		ParseMeshes(Scene);
	}

	void SkeletalModelImporter::ParseMeshes(const aiScene* Scene)
	{
		uint32_t TotalVertices = 0;
		uint32_t TotalIndices = 0;
		uint32_t TotalBones = 0;

		PreprocessMeshes(Scene, TotalVertices, TotalIndices, TotalBones);
		
		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			ParseSingleMesh(idx, Scene->mMeshes[idx], TotalVertices, TotalIndices, TotalBones);
		}
	}

	void SkeletalModelImporter::PreprocessMeshes(const aiScene* Scene, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount)
	{
		m_SkeletonData.MeshBaseVertex.resize(Scene->mNumMeshes);

		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			PreprocessSingleMesh(idx, Scene->mMeshes[idx], TotalVerticesCount, TotalIndicesCount, TotalBonesCount);
		}
	}

	void SkeletalModelImporter::PreprocessSingleMesh(uint16_t Index, const aiMesh* Mesh, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount)
	{
		m_SkeletonData.MeshBaseVertex[Index] = TotalVerticesCount;

		TotalVerticesCount += Mesh->mNumVertices;
		TotalIndicesCount += Mesh->mNumFaces * 3;
		TotalBonesCount += Mesh->mNumBones;

		m_SkeletonData.VertexToBones.resize(TotalVerticesCount);
	}

	void SkeletalModelImporter::ParseSingleMesh(uint16_t Index, const aiMesh* Mesh, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount)
	{
		if (Mesh->HasBones())
		{
			ParseMeshBones(Index, Mesh);
		}
	}

	void SkeletalModelImporter::ParseMeshBones(uint16_t MeshIndex, const aiMesh* Mesh)
	{
		for (uint16_t idx = 0; idx < Mesh->mNumBones; idx++)
		{
			ParseSingleBone(MeshIndex, Mesh->mBones[idx]);
		}
	}

	void SkeletalModelImporter::ParseSingleBone(uint16_t MeshIndex, const aiBone* Bone)
	{
		uint32_t BoneID = GetBoneID(Bone);

		for (unsigned long long idx = 0; idx < Bone->mNumWeights; idx++)
		{
			
			const aiVertexWeight& VertexWeight = Bone->mWeights[idx];
			
			uint32_t GlobalVertexId = m_SkeletonData.MeshBaseVertex[MeshIndex] + VertexWeight.mVertexId;
			Check(GlobalVertexId < m_SkeletonData.VertexToBones.size());
		
			m_SkeletonData.VertexToBones[GlobalVertexId].AddBoneData(GlobalVertexId, BoneID, VertexWeight.mWeight);
		}
	}

	uint32_t SkeletalModelImporter::GetBoneID(const aiBone* Bone)
	{
		uint32_t BoneID = 0;
		std::string BoneName = Bone->mName.C_Str();

		if (m_SkeletonData.BoneNameIndexMap.find(BoneName) != m_SkeletonData.BoneNameIndexMap.end())
		{
			BoneID = m_SkeletonData.BoneNameIndexMap[BoneName];
		}
		else
		{
			BoneID = m_SkeletonData.BoneNameIndexMap.size();
			m_SkeletonData.BoneNameIndexMap[BoneName] = BoneID;
		}

		return BoneID;
	}

	void VertexBoneData::AddBoneData(uint32_t VertID, uint32_t BoneID, float Weight)
	{
		for (uint16_t idx = 0; idx < s_MaxBonesPerVertex; idx++)
		{
			// Sanity check because importing with aiProcess_JoinIdenticalVertices produces duplicates of aiVertexWeight. 
			if (bool WeightAlreadyInvolved = std::find(Weights.cbegin(), Weights.cend(), Weight) != Weights.cend())
			{
				return;
			}

			if (Weights[idx] == 0.f)
			{
				//LOG(Assimp, Trace, "VertID {0} BoneID {1} Weight {2}",VertID, BoneID, Weight);
				BoneIDs[idx] = BoneID;
				Weights[idx] = Weight;
				return;
			}
		}
		CheckMsg(false,"Program not supports so many vertices influenced by one bone.");
	}
}
