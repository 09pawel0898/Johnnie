#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "AssetImporter.hpp"
#include "Log/Log.hpp"

#include "SkeletalMesh.hpp"

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

	SkeletalModelImporter::SkeletalModelImporter(TWeakPtr<ASkeletalMesh> SkeletalMesh)
	{
		m_SkeletalMesh = MoveTemp(SkeletalMesh);
		m_ImporterType = AssetImporterType::SkeletalModel;
	}

	void SkeletalModelImporter::AsyncImportModel(std::string_view FilePath)
	{
		uint32_t Flags = aiProcess_Triangulate
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
		m_SkeletonData.MeshBaseVertex.resize(Scene->mNumMeshes);

		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			PreprocessSingleMesh(idx, Scene->mMeshes[idx]);
		}
	}

	void SkeletalModelImporter::PreprocessSingleMesh(uint16_t Index, const aiMesh* AiMesh)
	{
		m_SkeletonData.MeshBaseVertex[Index] = m_ModelView.TotalVertices;

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
			TSharedPtr<Mesh> SubMesh = ParseSingleMeshData(AiMesh);
			SubMesh->SetStaticMeshOwner(SkeletalMesh);
			SubMesh->GetMeshStatistics().TrisCount = AiMesh->mNumFaces;

			SkeletalMesh->m_SubMeshes.emplace_back(MoveTemp(SubMesh));
		}
	}

	TSharedPtr<Mesh> SkeletalModelImporter::ParseSingleMeshData(const aiMesh* AiMesh)
	{
		std::vector<RHIVertex>	Vertices;
		std::vector<uint32_t>	Indices;

		Vertices.reserve(AiMesh->mNumVertices);
		Indices.reserve(AiMesh->mNumFaces * 3);

		// Process vertices //
		for (uint32_t i = 0; i < AiMesh->mNumVertices; i++)
		{
			RHIVertex Vertex;

			aiVector3D const& VertexPosition = AiMesh->mVertices[i];
			Vertex.Position = { VertexPosition.x, VertexPosition.y, VertexPosition.z };

			if (AiMesh->mNormals)
			{
				aiVector3D const& Normal = AiMesh->mNormals[i];
				Vertex.Normal = { Normal.x, Normal.y, Normal.z };
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

		TSharedPtr<Mesh> SubMesh = MakeShared<Mesh>(MoveTemp(Vertices),
													MoveTemp(Indices),
													true);

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
			BoneID = (uint32_t)m_SkeletonData.BoneNameIndexMap.size();
			m_SkeletonData.BoneNameIndexMap[BoneName] = BoneID;
		}

		return BoneID;
	}

	void VertexBoneData::AddBoneData(uint32_t VertID, uint32_t BoneID, float Weight)
	{
		for (uint8_t idx = 0; idx < s_MaxBonesPerVertex; idx++)
		{
			// Sanity check because importing with aiProcess_JoinIdenticalVertices produces duplicates of aiVertexWeight
			if (Weight == BoneInfluenceData[idx].Weight && BoneInfluenceData[idx].BoneID == BoneID)
			{
				return;
			}
			
			if (BoneInfluenceData[idx].Weight == 0.f)
			{
				//LOG(Assimp, Trace, "VertID {0} BoneID {1} Weight {2}",VertID, BoneID, Weight);
				BoneInfluenceData[idx].BoneID = BoneID;
				BoneInfluenceData[idx].Weight = Weight;
				return;
			}
		}
		CheckMsg(false,"Program not supports so many vertices influenced by one bone.");
	}
}
