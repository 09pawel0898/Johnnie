#include "EnginePCH.hpp"
#include "SkeletalMesh.hpp"
#include "Log/Log.hpp"
#include "AssimpHelpers.hpp"

namespace Engine
{
	ASkeletalMesh::ASkeletalMesh(std::string const& FilePath, OnSkeletalMeshAsyncLoadingFinishedDelegate OnLoadingFinished, glm::vec3 const& WorldLocation)
		:	Actor(WorldLocation),
			m_ModelFilePath(FilePath)
	{
		m_OnAsyncLoadingFinishedDelegate = MoveTemp(OnLoadingFinished);
	}

	ASkeletalMesh::~ASkeletalMesh()
	{}
	
	void ASkeletalMesh::Draw(void) const
	{
	}
	
	void ASkeletalMesh::OnTick(double DeltaTime)
	{
	}
	
	void ASkeletalMesh::OnConstruct(void)
	{
		m_LoadModelFuture = std::async(std::launch::async, std::bind(&ASkeletalMesh::ImportModel, this, std::placeholders::_1), m_ModelFilePath);
	}

	void ASkeletalMesh::ImportModel(std::string_view FilePath)
	{
		m_ModelImporter = AssetImporter::Create();

		uint32_t Flags = aiProcess_Triangulate 
			| aiProcess_OptimizeMeshes 
			| aiProcess_RemoveRedundantMaterials 
			| aiProcess_GenNormals 
			| aiProcess_JoinIdenticalVertices 
			| aiProcess_PreTransformVertices 
			| aiProcess_CalcTangentSpace;
		
		const aiScene* Scene = m_ModelImporter->GetImporter().ReadFile(FilePath.data(), Flags);
		
		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
		{
			LOG(Assimp, Error, "{0}", m_ModelImporter->GetImporter().GetErrorString());
			return;
		}
		m_Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
		
		InitializeMaterialSlots();
		ParseScene(Scene);
	}

	void ASkeletalMesh::Emplace_N_MaterialSlots(uint8_t N)
	{
		m_Materials.reserve(N);
		m_MaterialProcessed.reserve(N);

		for (uint8_t idx = 0; idx < N; idx++)
		{
			m_Materials.emplace_back(MakeShared<Material>());
			m_MaterialProcessed.emplace_back(false);
		}
		m_NumMaterials = N;
	}

	void ASkeletalMesh::ParseScene(const aiScene* Scene)
	{
		ParseMeshes(Scene);
	}

	void ASkeletalMesh::ParseMeshes(const aiScene* Scene)
	{
		uint32_t TotalVertices = 0;
		uint32_t TotalIndices = 0;
		uint32_t TotalBones = 0;

		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			ParseSingleMesh(Scene->mMeshes[idx],TotalVertices,TotalIndices,TotalBones);
		}
	}

	void ASkeletalMesh::ParseSingleMesh(const aiMesh* Mesh, uint32_t& TotalVerticesCount, uint32_t& TotalIndicesCount, uint32_t& TotalBonesCount)
	{
		TotalVerticesCount += Mesh->mNumVertices;
		TotalIndicesCount += Mesh->mNumFaces * 3;
		TotalBonesCount += Mesh->mNumBones;

		if (Mesh->HasBones())
		{
			ParseMeshBones(Mesh);
		}
	}

	void ASkeletalMesh::ParseMeshBones(const aiMesh* Mesh)
	{
		for (uint16_t idx = 0; idx < Mesh->mNumBones; idx++)
		{
			ParseSingleBone(idx, Mesh->mBones[idx]);
		}
	}

	void ASkeletalMesh::ParseSingleBone(uint16_t BoneIndex, const aiBone* Bone)
	{
		for (uint16_t idx = 0; idx < Bone->mNumWeights; idx++)
		{
			const aiVertexWeight& VertexWeight = Bone->mWeights[idx];
			LOG(Assimp,Trace,"VertId {0} Weight {1}",VertexWeight.mVertexId,VertexWeight.mWeight)
		}
	}

	void ASkeletalMesh::InitializeMaterialSlots()
	{
		m_Materials.clear();
		m_MaterialProcessed.clear();

		const aiScene* Scene = m_ModelImporter->GetScene();

		if (Scene != nullptr)
		{
			Emplace_N_MaterialSlots(Scene->mNumMaterials);
		}
		else
		{
			CheckMsg(false, "Scene was nullptr, can't parse materials info for initialization.");
		}
	}
}