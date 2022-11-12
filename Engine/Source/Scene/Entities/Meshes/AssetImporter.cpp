#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "AssetImporter.hpp"
#include "Log/Log.hpp"

#include "SkeletalMesh.hpp"

//#define DEBUG_MODEL_IMPORTER

namespace Engine
{
	Assimp::Importer& AssetImporter::GetImporter(void)
	{
		return m_AssimpImporter;
	}

	Assimp::Importer const& AssetImporter::GetImporter(void) const
	{
		return m_AssimpImporter;
	}

	const aiScene* AssetImporter::GetScene(void)
	{
		return m_AssimpImporter.GetScene();
	}

	bool AssetImporter::HasEmbededTextures(void)
	{
		return m_bHasEmbeddedTextures;
	}

	bool AssetImporter::IsModelAlreadyLoaded(void)
	{
		return m_bIsModelImported;
	}

	std::string const& AssetImporter::GetRootDirectory(void)
	{
		return m_RootDirectory;
	}

	aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHIMapTextureType RHIMapTextureType)
	{
		static std::unordered_map<RHI::RHIMapTextureType, aiTextureType> map =
		{
			{RHI::RHIMapTextureType::DiffuseMap,	aiTextureType_DIFFUSE	},
			{RHI::RHIMapTextureType::SpecularMap,	aiTextureType_SPECULAR	},
			{RHI::RHIMapTextureType::NormalMap,	aiTextureType_NORMALS	},
		};
		return map[RHIMapTextureType];
	}

	StaticModelImporter::StaticModelImporter()
	{
		m_ImporterType = AssetImporterType::StaticModel;
	}

	void StaticModelImporter::AsyncImportModel(std::string_view FilePath)
	{
	}

	SkeletalModelImporter::SkeletalModelImporter(TWeakPtr<ASkeletalMesh> SkeletalMesh)
	{
		m_SkeletalMesh = MoveTemp(SkeletalMesh);
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

		m_RootDirectory = FilePath.substr(0, FilePath.find_last_of('\\'));

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

		if (TSharedPtr<ASkeletalMesh> SkeletalMesh = m_SkeletalMesh.lock())
		{
			SkeletalMesh->InitializeMaterialSlots(Scene->mNumMaterials);
		}

		ParseScene(Scene);
		
		m_bIsModelImported = true;
	}

	void SkeletalModelImporter::ParseScene(const aiScene* Scene)
	{
		if (Scene->HasTextures())
		{
			m_bHasEmbeddedTextures = true;
		}

		ParseMeshes(Scene);
	}

	void SkeletalModelImporter::ParseMeshes(const aiScene* Scene)
	{
		PreprocessMeshes(Scene);
		
		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			ParseSingleMesh(idx, Scene->mMeshes[idx]);
		}
	}

	void SkeletalModelImporter::PreprocessMeshes(const aiScene* Scene)
	{
#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "----Preproces Meshes----");
#endif
		m_SkeletonData.MeshBaseVertex.resize(Scene->mNumMeshes);

		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			PreprocessSingleMesh(idx, Scene->mMeshes[idx]);
		}

#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "Total model vertices : {0}",m_ModelView.TotalVertices);
		LOG(Assimp, Trace, "Total model meshes : {0}",m_ModelView.Meshes.size());
#endif
	}

	void SkeletalModelImporter::PreprocessSingleMesh(uint16_t Index, const aiMesh* AiMesh)
	{
		m_SkeletonData.MeshBaseVertex[Index] = m_ModelView.TotalVertices;

#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "MeshBaseVertexForID {0} - {1}", Index, m_ModelView.TotalVertices);
#endif

		const uint32_t MeshVertices = AiMesh->mNumVertices;
		const uint32_t MeshIndices = AiMesh->mNumFaces * 3;
		const uint32_t MeshBones = AiMesh->mNumBones;

		m_ModelView.TotalVertices += MeshVertices;
		m_ModelView.TotalIndices += MeshIndices;
		m_ModelView.TotalBones += MeshBones;
		
		m_ModelView.Meshes.push_back({ Index,MeshVertices,MeshIndices,MeshBones });

		m_SkeletonData.VertexToBones.resize(m_ModelView.TotalVertices);
	}

	void SkeletalModelImporter::ParseSingleMesh(uint16_t Index, const aiMesh* AiMesh)
	{
		if (AiMesh->HasBones())
		{
			ParseMeshBones(Index, AiMesh);
		}

		if (TSharedPtr<ASkeletalMesh> SkeletalMesh = m_SkeletalMesh.lock())
		{
			TSharedPtr<SkinnedMesh> SubMesh = ParseSingleMeshData(Index, AiMesh);
			SubMesh->SetStaticMeshOwner(SkeletalMesh);
			SubMesh->GetMeshStatistics().TrisCount = AiMesh->mNumFaces;

			SkeletalMesh->m_SubMeshes.emplace_back(MoveTemp(SubMesh));
		}
	}

	TSharedPtr<SkinnedMesh> SkeletalModelImporter::ParseSingleMeshData(uint16_t Index, const aiMesh* AiMesh)
	{
		std::vector<RHIVertex>			Vertices;
		std::vector<uint32_t>			Indices;
		std::vector<VertexBoneData>		BoneData;

		Vertices.reserve(AiMesh->mNumVertices);
		BoneData.resize(AiMesh->mNumVertices);
		Indices.reserve(AiMesh->mNumFaces * 3);

		// Process vertices //
		for (uint32_t i = 0; i < AiMesh->mNumVertices; i++)
		{
			RHIVertex Vertex;

			aiVector3D const& VertexPosition = AiMesh->mVertices[i];
			Vertex.Position = { VertexPosition.x, VertexPosition.y, VertexPosition.z };

			if (AiMesh->mNormals)
			{
				aiVector3D const& NormalMap = AiMesh->mNormals[i];
				Vertex.NormalMap = { NormalMap.x, NormalMap.y, NormalMap.z };
			}

			if (AiMesh->mTangents)
			{
				aiVector3D const& Tangent = AiMesh->mTangents[i];
				Vertex.Tangent = { Tangent.x, Tangent.y, Tangent.z };
			}

			if (AiMesh->mTextureCoords[0])
			{
				aiVector3D const& TextureCoords = AiMesh->mTextureCoords[0][i];
				Vertex.TexUV = { TextureCoords.x, TextureCoords.y };
			}
			else
			{
				Vertex.TexUV = { 0.f, 0.f };
			}
			Vertices.emplace_back(MoveTemp(Vertex));
		}

		// Process indices //
		for (uint32_t i = 0; i < AiMesh->mNumFaces; i++)
		{
			aiFace const& face = AiMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				Indices.emplace_back(face.mIndices[j]);
			}
		}
		
		uint32_t Offset = m_SkeletonData.MeshBaseVertex[Index];

#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "--- Creating SkinnedMesh --- ID {0}", Index);
		LOG(Assimp, Trace, "Total Vertices : {0}",AiMesh->mNumVertices);
		
		LOG(Assimp, Trace, "VertexToBones.First : {0}", Offset);
		LOG(Assimp, Trace, "VertexToBones.Last : {0}", Offset + AiMesh->mNumVertices);

		//if(Index == 0)
		//{
		//	for (auto	iter = m_SkeletonData.VertexToBones.begin() + Offset; 
		//				iter < m_SkeletonData.VertexToBones.begin() + Offset + AiMesh->mNumVertices; 
		//				iter++)
		//	{
		//		for (int boneId = 0; boneId < 4; boneId++)
		//		{
		//			if (iter->BoneIDs[boneId] != -1)
		//			{
		//				LOG(Assimp, Trace, "Mesh ID {0} VertexID {1} InfluencedBy -> Bone {2} Weight {3}",Index,std::distance(m_SkeletonData.VertexToBones.begin(),iter),iter->BoneIDs[boneId],iter->Weights[boneId]);
		//			}
		//		}
		//	}
		//}
#endif
		std::copy(	m_SkeletonData.VertexToBones.begin() + Offset,
					m_SkeletonData.VertexToBones.begin() + Offset + AiMesh->mNumVertices, 
					BoneData.begin());
		
		TSharedPtr<SkinnedMesh> SubMesh = MakeShared<SkinnedMesh>(	MoveTemp(Vertices),
																	MoveTemp(Indices),
																	MoveTemp(BoneData));

		// Process materials //
		if (AiMesh->mMaterialIndex >= 0)
		{
			SubMesh->SetMaterialIndex(AiMesh->mMaterialIndex);
		}

		return SubMesh;
	}

	void SkeletalModelImporter::ParseMeshBones(uint16_t MeshIndex, const aiMesh* AiMesh)
	{
		for (uint16_t idx = 0; idx < AiMesh->mNumBones; idx++)
		{
			ParseSingleBone(MeshIndex, AiMesh->mBones[idx]);
		}
	}

	void SkeletalModelImporter::ParseSingleBone(uint16_t MeshIndex, const aiBone* Bone)
	{
		uint32_t BoneID = GetBoneID(Bone);

#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "Bone {0} ID {1}, NumVerticesAffected {2}", Bone->mName.C_Str(), BoneID, Bone->mNumWeights)	
#endif

		for (unsigned long long idx = 0; idx < Bone->mNumWeights; idx++)
		{
			const aiVertexWeight& VertexWeight = Bone->mWeights[idx];
			
			uint32_t GlobalVertexID = m_SkeletonData.MeshBaseVertex[MeshIndex] + VertexWeight.mVertexId;
			Check(GlobalVertexID < m_SkeletonData.VertexToBones.size());
			
			m_SkeletonData.VertexToBones[GlobalVertexID].AddBoneData(GlobalVertexID, BoneID, VertexWeight.mWeight);
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
			BoneID = (uint32_t)m_SkeletonData.BoneNameIndexMap.size();
			m_SkeletonData.BoneNameIndexMap[BoneName] = BoneID;
		}

		return BoneID;
	}

	void VertexBoneData::AddBoneData(uint32_t GlobalVertexID, uint32_t BoneID, float Weight)
	{
		for (uint8_t idx = 0; idx < Index; idx++)
		{
			// Sanity check because importing with aiProcess_JoinIdenticalVertices produces duplicates of aiVertexWeight
			if (BoneIDs[idx] == BoneID)
			{
				return;
			}
		}

		if (Weight == 0.f)
		{
			return;
		}

		CheckMsg(Index != s_MaxBonesPerVertex,"Program not supports so many vertices influenced by one bone.");
		
		BoneIDs[Index] = BoneID;
		Weights[Index] = Weight;

#ifdef DEBUG_MODEL_IMPORTER
		//LOG(Assimp, Trace, "GlobalVertexID {0} BoneID {1} Weight {2} InfoSlotIndex {3}", GlobalVertexID, BoneIDs[Index], Weights[Index], Index);
#endif
		Index++;
	}
}
