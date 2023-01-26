#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "AssetImporter.hpp"
#include "Log/Log.hpp"
#include "Utilities/GLMUtility.hpp"
#include "Utilities/VariantUtility.hpp"
#include "SkeletalMesh/Animation.hpp"

#include "SkeletalMesh/SkeletalMesh.hpp"

namespace Engine
{
	namespace Utility
	{
		static void ScaleScene(const aiScene* Scene, float ScaleFactor = 1.f);

		static void TraverseNodes(aiNode* Node, float ScaleFactor, unsigned int NestedNodeID = 0);

		static void ApplyScaling(aiNode* CurrentNode, float ScaleFactor);

		static void ScaleScene(const aiScene* Scene, float ScaleFactor)
		{
			if (ScaleFactor == 1.0f) {
				return; // nothing to scale
			}

			Check(ScaleFactor != 0);
			Check(nullptr != Scene);
			Check(nullptr != Scene->mRootNode);

			if (nullptr == Scene) {
				return;
			}

			if (nullptr == Scene->mRootNode) {
				return;
			}

			// Process animations and update position transform to new unit system
			for (unsigned int animationID = 0; animationID < Scene->mNumAnimations; animationID++)
			{
				aiAnimation* animation = Scene->mAnimations[animationID];

				for (unsigned int animationChannel = 0; animationChannel < animation->mNumChannels; animationChannel++)
				{
					aiNodeAnim* anim = animation->mChannels[animationChannel];

					for (unsigned int posKey = 0; posKey < anim->mNumPositionKeys; posKey++)
					{
						aiVectorKey& vectorKey = anim->mPositionKeys[posKey];
						vectorKey.mValue *= ScaleFactor;
					}
				}
			}

			for (unsigned int meshID = 0; meshID < Scene->mNumMeshes; meshID++)
			{
				aiMesh* mesh = Scene->mMeshes[meshID];

				// Reconstruct mesh vertices to the new unit system
				for (unsigned int vertexID = 0; vertexID < mesh->mNumVertices; vertexID++)
				{
					aiVector3D& vertex = mesh->mVertices[vertexID];
					vertex *= ScaleFactor;
				}


				// bone placement / scaling
				for (unsigned int boneID = 0; boneID < mesh->mNumBones; boneID++)
				{
					// Reconstruct matrix by transform rather than by scale
					// This prevent scale values being changed which can
					// be meaningful in some cases
					// like when you want the modeller to see 1:1 compatibility.
					aiBone* bone = mesh->mBones[boneID];

					aiVector3D pos, scale;
					aiQuaternion rotation;

					bone->mOffsetMatrix.Decompose(scale, rotation, pos);

					aiMatrix4x4 translation;
					aiMatrix4x4::Translation(pos * ScaleFactor, translation);

					aiMatrix4x4 scaling;
					aiMatrix4x4::Scaling(aiVector3D(scale), scaling);

					aiMatrix4x4 RotMatrix = aiMatrix4x4(rotation.GetMatrix());

					bone->mOffsetMatrix = translation * RotMatrix * scaling;
				}


				// animation mesh processing
				// convert by position rather than scale.
				for (unsigned int animMeshID = 0; animMeshID < mesh->mNumAnimMeshes; animMeshID++)
				{
					aiAnimMesh* animMesh = mesh->mAnimMeshes[animMeshID];

					for (unsigned int vertexID = 0; vertexID < animMesh->mNumVertices; vertexID++)
					{
						aiVector3D& vertex = animMesh->mVertices[vertexID];
						vertex *= ScaleFactor;
					}
				}
			}

			TraverseNodes(Scene->mRootNode, ScaleFactor);
		}

		static void TraverseNodes(aiNode* Node, float ScaleFactor, unsigned int NestedNodeID)
		{
			ApplyScaling(Node, ScaleFactor);

			for (size_t i = 0; i < Node->mNumChildren; i++)
			{
				TraverseNodes(Node->mChildren[i], ScaleFactor, NestedNodeID + 1);
			}
		}

		static void ApplyScaling(aiNode* CurrentNode, float ScaleFactor)
		{
			if (nullptr != CurrentNode)
			{
				aiVector3D pos, scale;
				aiQuaternion rotation;
				CurrentNode->mTransformation.Decompose(scale, rotation, pos);

				aiMatrix4x4 translation;
				aiMatrix4x4::Translation(pos * ScaleFactor, translation);

				aiMatrix4x4 scaling;

				aiMatrix4x4::Scaling(scale, scaling);

				aiMatrix4x4 RotMatrix = aiMatrix4x4(rotation.GetMatrix());

				CurrentNode->mTransformation = translation * RotMatrix * scaling;
			}
		}

	}
	Assimp::Importer& AssetImporter::GetImporter(void)
	{

		return m_AssimpImporter;
	}

	Assimp::Importer const& AssetImporter::GetImporter(void) const
	{
		return m_AssimpImporter;
	}

	const aiScene* AssetImporter::GetScene(void) const
	{
		return m_AssimpImporter.GetScene();
	}

	bool ModelImporter::HasEmbededTextures(void)
	{
		return m_bHasEmbeddedTextures;
	}

	bool ModelImporter::IsModelAlreadyLoaded(void)
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
		uint32_t Flags = aiProcess_Triangulate
			| aiProcess_RemoveRedundantMaterials
			| aiProcess_OptimizeMeshes
			| aiProcess_GenNormals
			| aiProcess_JoinIdenticalVertices
			| aiProcess_CalcTangentSpace
			;

		m_RootDirectory = FilePath.substr(0, FilePath.find_last_of('\\'));
		//m_RootDirectory = FilePath.substr(0, FilePath.find_last_of('/'));

		m_ImportModelFuture = std::async(std::launch::async,
			std::bind(&SkeletalModelImporter::AsyncImportModel_Internal, this, std::placeholders::_1, std::placeholders::_2),
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
		Utility::ScaleScene(Scene, 0.020f);

		ParseScene(Scene);

		m_bIsModelImported = true;
	}

	void SkeletalModelImporter::ParseScene(const aiScene* Scene)
	{
		if (Scene->HasTextures())
		{
			m_bHasEmbeddedTextures = true;
		}

		ParseHierarchyData(Scene->mRootNode);
		ParseMeshes(Scene);

		TSharedPtr<ASkeletalMesh> SkeletalMesh = m_SkeletalMesh.lock();
		Check(SkeletalMesh);

		m_Skeleton.SetGlobalInverseTransform(glm::inverse(Utility::AiMat4ToGlmMat4(Scene->mRootNode->mTransformation)));
		SkeletalMesh->SetSkeleton(m_Skeleton);
	}

	void SkeletalModelImporter::ParseMeshes(const aiScene* Scene)
	{
		PreprocessMeshes(Scene);

		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			ParseSingleMesh(idx, Scene->mMeshes[idx]);
		}

#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "-------- Model {0} skeleton --------", Scene->mName.C_Str());
		PrintSkeleton(GetScene()->mRootNode);
#endif
	}

	void SkeletalModelImporter::PreprocessMeshes(const aiScene* Scene)
	{
#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "----Preproces Meshes----");
#endif
		m_SkinnedMeshData.MeshBaseVertex.resize(Scene->mNumMeshes);

		for (uint16_t idx = 0; idx < Scene->mNumMeshes; idx++)
		{
			PreprocessSingleMesh(idx, Scene->mMeshes[idx]);
		}

#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "Total model vertices : {0}", m_ModelView.TotalVertices);
		LOG(Assimp, Trace, "Total model meshes : {0}", m_ModelView.Meshes.size());
#endif
	}

	void SkeletalModelImporter::PreprocessSingleMesh(uint16_t Index, const aiMesh* AiMesh)
	{
		m_SkinnedMeshData.MeshBaseVertex[Index] = m_ModelView.TotalVertices;

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

		m_SkinnedMeshData.VertexBoneInfluenceData.resize(m_ModelView.TotalVertices);
	}

	void SkeletalModelImporter::ParseSingleMesh(uint16_t Index, const aiMesh* AiMesh)
	{
#ifdef DEBUG_MODEL_IMPORTER
		LOG(Core, Trace, "Mesh name {0}", AiMesh->mName.C_Str());
#endif
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
		std::vector<RHIVertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<VertexBoneInfluenceData> BoneData;

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

		uint32_t Offset = m_SkinnedMeshData.MeshBaseVertex[Index];

		std::copy(	m_SkinnedMeshData.VertexBoneInfluenceData.begin() + Offset,
					m_SkinnedMeshData.VertexBoneInfluenceData.begin() + Offset + AiMesh->mNumVertices,
					BoneData.begin());

		TSharedPtr<SkinnedMesh> SubMesh = MakeShared<SkinnedMesh>(MoveTemp(Vertices),
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

	void SkeletalModelImporter::ParseSingleBone(uint16_t MeshIndex, const aiBone* AnimationKeyFrame)
	{
		MarkRequiredNodesForBone(AnimationKeyFrame);

		uint32_t BoneID = GetBoneID(AnimationKeyFrame);

#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "Parse Bone {0} ID {1}, NumVerticesAffected {2}", AnimatedBoneData->mName.C_Str(), BoneID, AnimatedBoneData->mNumWeights)
#endif
		if (BoneID == m_Skeleton.GetBonesData().size())
		{
			m_Skeleton.GetBonesData().emplace_back(BoneData(Utility::AiMat4ToGlmMat4(AnimationKeyFrame->mOffsetMatrix)));
		}

		for (unsigned long long idx = 0; idx < AnimationKeyFrame->mNumWeights; idx++)
		{
			const aiVertexWeight& VertexWeight = AnimationKeyFrame->mWeights[idx];

			uint32_t GlobalVertexID = m_SkinnedMeshData.MeshBaseVertex[MeshIndex] + VertexWeight.mVertexId;
			Check(GlobalVertexID < m_SkinnedMeshData.VertexBoneInfluenceData.size());

			m_SkinnedMeshData.VertexBoneInfluenceData[GlobalVertexID].AddBoneData(GlobalVertexID, BoneID, VertexWeight.mWeight);
		}
	}

	void SkeletalModelImporter::ParseHierarchyData(const aiNode* AiRootNode)
	{
		ParseHierarchyData_BuildHierarchy(&m_Skeleton.GetRootNode(), AiRootNode);
		ParseHierarchyData_FixParenting(&m_Skeleton.GetRootNode());
	}

	void SkeletalModelImporter::ParseHierarchyData_BuildHierarchy(NodeData* Node, const aiNode* AiNode)
	{
		Check(Node);
		
		Node->Name				= AiNode->mName.data;
		Node->Transformation	= Utility::AiMat4ToGlmMat4(AiNode->mTransformation);
		Node->ChildrenCount		= AiNode->mNumChildren;
		
		for (uint16_t idx = 0; idx < AiNode->mNumChildren; idx++)
		{
			NodeData NewNode;
			NewNode.Children.reserve(AiNode->mNumChildren);

			ParseHierarchyData_BuildHierarchy(&NewNode, AiNode->mChildren[idx]);
			Node->Children.emplace_back(MoveTemp(NewNode));
		}
	}

	void SkeletalModelImporter::ParseHierarchyData_FixParenting(NodeData* Node)
	{
		for (uint16_t idx = 0; idx < Node->ChildrenCount; idx++)
		{
			Node->Children[idx].Parent = Node;
			ParseHierarchyData_FixParenting(&Node->Children[idx]);
		}
	}

	void SkeletalModelImporter::PrintSkeleton(aiNode* Node, uint16_t Deep /*=0*/)
	{
		const aiScene* Scene = GetScene();

		const char* BoneName = Node->mName.C_Str();
		std::string Message = "";

		for (uint16_t idx = 0; idx < Deep; idx++)
		{
			Message += "  ";
		}

		if (m_Skeleton.GetBoneNameIndexMap().find(BoneName) != m_Skeleton.GetBoneNameIndexMap().end())
		{
			Message += "[B] " + std::string(Node->mName.C_Str());
		}
		else
		{
			Message += std::string(Node->mName.C_Str());
		}
		
		LOG(Assimp, Trace, "{0}" ,Message);

		for (uint16_t idx = 0; idx < Node->mNumChildren; idx++)
		{
			PrintSkeleton(Node->mChildren[idx],Deep+1);
		}
	}

	uint32_t SkeletalModelImporter::GetBoneID(const aiBone* AnimationKeyFrame)
	{
		uint32_t BoneID = 0;
		std::string BoneName = AnimationKeyFrame->mName.C_Str();

		if (m_Skeleton.GetBoneNameIndexMap().find(BoneName) != m_Skeleton.GetBoneNameIndexMap().end())
		{
			BoneID = m_Skeleton.GetBoneNameIndexMap()[BoneName];
		}
		else
		{
			BoneID = (uint32_t)m_Skeleton.GetBoneNameIndexMap().size();
			m_Skeleton.GetBoneNameIndexMap()[BoneName] = BoneID;
		}

		return BoneID;
	}

	void SkeletalModelImporter::MarkRequiredNodesForBone(const aiBone* AiBone)
	{
		std::string NodeName = AiBone->mName.C_Str();
		const NodeData* Parent = nullptr;

		do
		{
			NodeData* FoundNode = m_Skeleton.FindNodeByName(NodeName);
			CheckMsg(FoundNode != nullptr, "Can't find bone in the node hierarchy.");
			
			FoundNode->IsRequired = true;
			Parent = FoundNode->Parent;

			if (Parent)
			{
				NodeName = Parent->Name;
			}
		} while (Parent);
	}

	void AnimationImporter::AsyncImportFirstAnimation(TWeakPtr<ASkeletalMesh> SkeletalMesh, std::string const& FilePath, OnAnimationAsyncLoadingFinishedDelegate OnAsyncLoadingFinished)
	{
		if (m_bIsBusy)
		{
			LOG(Assimp, Error, "Can't import animation asset, importer busy.");
			return;
		}

		m_SkeletalMesh = SkeletalMesh;
		m_bIsBusy = true;

		m_OnAsyncLoadingFinished = MoveTemp(OnAsyncLoadingFinished);
		m_RootDirectory = FilePath.substr(0, FilePath.find_last_of('\\'));

		m_ReadSceneFuture = std::async(	std::launch::async,
										std::bind(&AnimationImporter::AsyncImportScene_Internal, this, std::placeholders::_1, std::placeholders::_2),
										FilePath, false);
	}

	void AnimationImporter::AsyncImportAllAnimations(TWeakPtr<ASkeletalMesh> SkeletalMesh, std::string const& FilePath, OnAnimationsAsyncLoadingFinishedDelegate OnAsyncLoadingFinished)
	{
		if (m_bIsBusy)
		{
			LOG(Assimp, Error, "Can't import animation assets, importer busy.");
			return;
		}

		m_SkeletalMesh = SkeletalMesh;
		m_bIsBusy = true;

		m_OnAsyncLoadingFinished = MoveTemp(OnAsyncLoadingFinished);
		m_RootDirectory = FilePath.substr(0, FilePath.find_last_of('\\'));

		m_ReadSceneFuture = std::async(	std::launch::async,
										std::bind(&AnimationImporter::AsyncImportScene_Internal, this, std::placeholders::_1, std::placeholders::_2),
										FilePath, true);
	}

	void AnimationImporter::AsyncImportScene_Internal(std::string_view FilePath, bool ImportAllAnimations)
	{
		const uint32_t Flags = aiProcess_Triangulate;

		const aiScene* Scene = m_AssimpImporter.ReadFile(FilePath.data(), Flags);

		if (!Scene || !Scene->mRootNode)
		{
			LOG(Assimp, Error, "{0}", m_AssimpImporter.GetErrorString());
			return;
		}

		if (!Scene->HasAnimations())
		{
			LOG(Assimp, Warning, "Animation import failed. No animation data found in file [{0}]", FilePath);
			return;
		}

		Utility::ScaleScene(Scene, 0.020f);

		uint8_t NumAnimationsToImport = (ImportAllAnimations) ? Scene->mNumAnimations : 1;

		m_ImportAnimationFutures.resize(NumAnimationsToImport);
		m_LoadedAnimations.reserve(NumAnimationsToImport);

		// Load all animations asynchronously
		for (uint16_t idx = 0; idx < NumAnimationsToImport; idx++)
		{
			const aiAnimation* AnimationToParse = Scene->mAnimations[idx];

			m_ImportAnimationFutures[idx] = std::async(std::launch::async,
				std::bind(&AnimationImporter::AsyncImportAnimation_Internal, this, std::placeholders::_1),
				AnimationToParse);
		}

		// Wait for all animations to be loaded 
		for (uint16_t futureIdx = 0; futureIdx < m_ImportAnimationFutures.size(); futureIdx++)
		{
			m_ImportAnimationFutures[futureIdx].wait();
		}
		NotifyLoadingFinished();

		m_bIsBusy = false;
	}

	void AnimationImporter::AsyncImportAnimation_Internal(const aiAnimation* AiAnimation)
	{
		const aiScene* Scene = GetScene();

		{
			std::scoped_lock lock(m_LoadedAnimationsLock);
			m_LoadedAnimations.emplace_back(Animation());
		}

		Animation& NewAnim = m_LoadedAnimations[m_LoadedAnimations.size() - 1];
		
		NewAnim.m_Duration = (float)AiAnimation->mDuration;
		NewAnim.m_TicksPerSecond = (float)AiAnimation->mTicksPerSecond;

		ReadBonesData(&NewAnim, AiAnimation);
		
#ifdef DEBUG_MODEL_IMPORTER
		LOG(Assimp, Trace, "-------- Animation {0} skeleton --------", AiAnimation->mName.C_Str());
		PrintSkeleton(m_LoadedAnimations.size() - 1,Scene->mRootNode);
#endif
	}

	void AnimationImporter::PrintSkeleton(uint16_t AnimationIndex, aiNode* Node, uint16_t Deep /*=0*/)
	{
		TSharedPtr<ASkeletalMesh> SkeletalMesh = m_SkeletalMesh.lock();
		Check(SkeletalMesh);

		const aiScene* Scene = GetScene();
		const char* BoneName = Node->mName.C_Str();
		std::string Message = "";

		for (uint16_t idx = 0; idx < Deep; idx++)
		{
			Message += "  ";
		}

		Animation& Animation = m_LoadedAnimations[AnimationIndex];

		if (Animation.GetBoneNameToIndexMap().find(BoneName) != Animation.GetBoneNameToIndexMap().end())
		{
			Message += "[B] " + std::string(Node->mName.C_Str());
		}
		else
		{
			Message += std::string(Node->mName.C_Str());
		}

		LOG(Assimp, Trace, "{0}", Message);

		for (uint16_t idx = 0; idx < Node->mNumChildren; idx++)
		{
			PrintSkeleton(AnimationIndex,Node->mChildren[idx], Deep + 1);
		}
	}

	void AnimationImporter::ReadBonesData(Animation* Anim, const aiAnimation* AiAnimation)
	{
		TSharedPtr<ASkeletalMesh> SkeletalMesh = m_SkeletalMesh.lock();
		Check(SkeletalMesh);

		std::map<std::string, uint32_t> BoneNameIndexMap = SkeletalMesh->GetSkeleton().GetBoneNameIndexMap();
		uint32_t BoneID = 0;

		for (uint16_t idx = 0; idx < AiAnimation->mNumChannels; idx++)
		{
			aiNodeAnim* NodeAnim = AiAnimation->mChannels[idx];
			const std::string BoneName = NodeAnim->mNodeName.data;

			if (BoneNameIndexMap.find(BoneName) == BoneNameIndexMap.end())
			{
				BoneID = (uint32_t)BoneNameIndexMap.size();
				BoneNameIndexMap[BoneName] = BoneID;

				LOG(Core, Trace, "Extra Bone In Anim {0}", BoneName);
			}

			Anim->m_BoneKeyFrames.push_back(AnimationBoneKeyFrames(NodeAnim->mNodeName.data,BoneNameIndexMap[BoneName], NodeAnim));
		}
		Anim->m_BoneNameIndexMap = BoneNameIndexMap;
	}

	/*
	aiNode* AnimationImporter::GetRootBone(aiNode* SceneRootNode)
	{
		aiNode* RootBone = nullptr;
	
		FindRootBone(SceneRootNode,&RootBone);
		return RootBone;
	}
	
	void AnimationImporter::FindRootBone(aiNode* Node, aiNode** OutResult)
	{
		if (*OutResult != nullptr)
		{
			return;
		}
	
		auto IsBone = [this](std::string const& NodeName) -> bool
		{
			return (m_BoneNameIndexMap.find(NodeName) != m_BoneNameIndexMap.end()) ? true : false;
		};
	
		std::string NodeName = Node->mName.C_Str();
	
		if (IsBone(NodeName))
		{
			if (Node->mParent == nullptr)
			{
				return;
			}
	
			std::string ParentName = Node->mParent->mName.C_Str();
			if (!IsBone(ParentName))
			{
				*OutResult = Node;
			}
		}
	
		for (uint16_t idx = 0; idx < Node->mNumChildren; idx++)
		{
			FindRootBone(Node->mChildren[idx], OutResult);
		}
	}
	*/
	void AnimationImporter::NotifyLoadingFinished(void)
	{
		std::visit(MakeInlineVisitor(
			[this](OnAnimationAsyncLoadingFinishedDelegate& OnAsyncLoadingFinished) 
			{
				CheckMsg(m_LoadedAnimations.size() == 1, "Trying to send notification about 1 loaded animation but loaded animations count != 1");
				OnAsyncLoadingFinished.ExecuteIfBound(m_LoadedAnimations[0]);
				m_LoadedAnimations.clear();
			},
			[this](OnAnimationsAsyncLoadingFinishedDelegate& OnAsyncLoadingFinished)
			{
				CheckMsg(m_LoadedAnimations.size() >= 1, "Trying to send notification about >= 1 loaded animation but loaded animations count < 1");
				OnAsyncLoadingFinished.ExecuteIfBound(m_LoadedAnimations);
				m_LoadedAnimations.clear();
			}), m_OnAsyncLoadingFinished);
	}
}