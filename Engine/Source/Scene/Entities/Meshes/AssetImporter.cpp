#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "AssetImporter.hpp"
#include "Log/Log.hpp"
#include "Utilities/GLMUtility.hpp"

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

		m_GlobalInverseTransform = glm::inverse(Utility::AiMat4ToGlmMat4(Scene->mRootNode->mTransformation));
		
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

		m_SkeletonData.VertexBoneInfluenceData.resize(m_ModelView.TotalVertices);
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
		std::vector<VertexBoneInfluenceData>		BoneData;

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
		std::copy(	m_SkeletonData.VertexBoneInfluenceData.begin() + Offset,
					m_SkeletonData.VertexBoneInfluenceData.begin() + Offset + AiMesh->mNumVertices, 
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

		if (BoneID == m_SkeletonData.BonesData.size())
		{
			m_SkeletonData.BonesData.emplace_back(BoneData(Utility::AiMat4ToGlmMat4(Bone->mOffsetMatrix)));
		}

		for (unsigned long long idx = 0; idx < Bone->mNumWeights; idx++)
		{
			const aiVertexWeight& VertexWeight = Bone->mWeights[idx];
			
			uint32_t GlobalVertexID = m_SkeletonData.MeshBaseVertex[MeshIndex] + VertexWeight.mVertexId;
			Check(GlobalVertexID < m_SkeletonData.VertexBoneInfluenceData.size());
			
			m_SkeletonData.VertexBoneInfluenceData[GlobalVertexID].AddBoneData(GlobalVertexID, BoneID, VertexWeight.mWeight);
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

	void SkeletalModelImporter::GetBoneTransforms(float AnimationTimeInSeconds, std::vector<glm::mat4>& OutTransformMatrices)
	{
		const aiScene* Scene = GetScene();

		if (GetScene()->HasAnimations())
		{
			const float TicksPerSecond = (float)(Scene->mAnimations[0]->mTicksPerSecond != 0 ? Scene->mAnimations[0]->mTicksPerSecond : 25.0f);
			const float TimeInTicks = AnimationTimeInSeconds * TicksPerSecond;
			const float AnimationTimeTicks = fmod(TimeInTicks, (float)Scene->mAnimations[0]->mDuration); // loop animation

			ReadNodeHierarchy(AnimationTimeTicks, GetScene()->mRootNode, glm::mat4(1));
		}
		else
		{
			ReadNodeHierarchy(0.f, GetScene()->mRootNode, glm::mat4(1));
		}

		OutTransformMatrices.resize(m_SkeletonData.BonesData.size());

		for (uint16_t idx = 0; idx < m_SkeletonData.BonesData.size(); idx++)
		{
			OutTransformMatrices[idx] = m_SkeletonData.BonesData[idx].FinalTransformation;
		}
	}

	void SkeletalModelImporter::ReadNodeHierarchy(float AnimationTimeInTicks, const aiNode* Node, glm::mat4 const& ParentTransformMatrix)
	{
		std::string const& NodeName = Node->mName.data;
		glm::mat4 NodeTransformation = Utility::AiMat4ToGlmMat4(Node->mTransformation);
		
		const aiScene* Scene = GetScene();

		if (Scene->HasAnimations())
		{
			const aiAnimation* Animation = Scene->mAnimations[0];
		
			if (const aiNodeAnim* NodeAnim = FindNodeAnim(Animation, NodeName))
			{
				const glm::mat4 Identity = glm::mat4(1);

				// Interpolate scaling and generate scaling transformation matrix
				aiVector3D Scaling;
				CalculateInterpolatedScaling(Scaling, AnimationTimeInTicks, NodeAnim);

				aiMatrix4x4 SM;
				aiMatrix4x4::Scaling(Scaling, SM);

				glm::mat4 ScaleMatrix = glm::scale(Identity,glm::vec3(Scaling.x, Scaling.y, Scaling.z));
			
				// Interpolate rotation and generate rotation transformation matrix
				aiQuaternion Rotation;
				CalculateInterpolatedRotation(Rotation, AnimationTimeInTicks, NodeAnim);

				aiMatrix4x4 RM = aiMatrix4x4(Rotation.GetMatrix());

				glm::mat4 RotationMatrix = glm::mat4(1.f);

				
				//RotationMatrix = glm::rotate(RotationMatrix, AngleX, glm::vec3(1.0f, 0.0f, 0.0f));
				//RotationMatrix = glm::rotate(RotationMatrix, AngleY, glm::vec3(0.0f, 1.0f, 0.0f));
				//RotationMatrix = glm::rotate(RotationMatrix, AngleZ, glm::vec3(0.0f, 0.0f, 1.0f));
				//glm::mat4 RotationMatrix = Utility::AiQuatToGlmRotationMatrix(Rotation);
				//glm::mat4 RotationMatrix = Utility::AiMat3ToGlmMat4(Rotation.GetMatrix());

				// Interpolate translation and generate translation transformation matrix
				aiVector3D Translation;
				CalculateInterpolatedLocation(Translation, AnimationTimeInTicks, NodeAnim);

				aiMatrix4x4 TM;
				aiMatrix4x4::Translation(Translation, TM);

				glm::mat4 TranslationMatrix = glm::translate(Identity, glm::vec3(Translation.x, Translation.y, Translation.z));
				
				// Combine the above transformations
				NodeTransformation = Utility::AiMat4ToGlmMat4(TM * RM * SM);
			}
		}

		glm::mat4 GlobalTransformation = ParentTransformMatrix * NodeTransformation;

		if (m_SkeletonData.BoneNameIndexMap.find(NodeName) != m_SkeletonData.BoneNameIndexMap.end()) 
		{
			uint32_t BoneIndex = m_SkeletonData.BoneNameIndexMap[NodeName];
			m_SkeletonData.BonesData[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_SkeletonData.BonesData[BoneIndex].OffsetMatrix;
		}

		for (uint32_t idx = 0; idx < Node->mNumChildren; idx ++)
		{
			ReadNodeHierarchy(AnimationTimeInTicks, Node->mChildren[idx], GlobalTransformation);
		}
	}

	const aiNodeAnim* SkeletalModelImporter::FindNodeAnim(const aiAnimation* Animation, std::string const& NodeName)
	{
		for (uint16_t idx = 0; idx < Animation->mNumChannels; idx++)
		{
			const aiNodeAnim* NodeAnim = Animation->mChannels[idx];

			if (NodeName == std::string(NodeAnim->mNodeName.data))
			{
				return NodeAnim;
			}
		}
		return nullptr;
	}

	uint32_t SkeletalModelImporter::FindPosition(float AnimationTimeInTicks, const aiNodeAnim* NodeAnim)
	{
		Check(NodeAnim->mNumPositionKeys > 0);

		for (uint32_t idx = 0; idx < NodeAnim->mNumPositionKeys - 1; idx++) 
		{
			float TimeForKey = (float)NodeAnim->mPositionKeys[idx + 1].mTime;

			if (AnimationTimeInTicks < TimeForKey)
			{
				return idx;
			}
		}

		return 0;
	}

	uint32_t SkeletalModelImporter::FindRotation(float AnimationTimeInTicks, const aiNodeAnim* NodeAnim)
	{
		Check(NodeAnim->mNumRotationKeys > 0);

		for (uint32_t idx = 0; idx < NodeAnim->mNumRotationKeys - 1; idx++) 
		{
			float TimeForKey = (float)NodeAnim->mRotationKeys[idx + 1].mTime;

			if (AnimationTimeInTicks < TimeForKey)
			{
				return idx;
			}
		}

		return 0;
	}

	uint32_t SkeletalModelImporter::FindScaling(float AnimationTimeInTicks, const aiNodeAnim* NodeAnim)
	{
		Check(NodeAnim->mNumScalingKeys > 0);

		for (uint32_t idx = 0; idx < NodeAnim->mNumScalingKeys - 1; idx++) 
		{
			float TimeForKey = (float)NodeAnim->mScalingKeys[idx + 1].mTime;

			if (AnimationTimeInTicks < TimeForKey)
			{
				return idx;
			}
		}
		return 0;
	}

	void SkeletalModelImporter::CalculateInterpolatedScaling(aiVector3D& OutScale, float AnimationTimeInTicks, const aiNodeAnim* NodeAnim)
	{
		// At least two values are needed to interpolate
		if (NodeAnim->mNumScalingKeys == 1) 
		{
			OutScale = NodeAnim->mScalingKeys[0].mValue;
			return;
		}

		uint32_t ScalingIndex = FindScaling(AnimationTimeInTicks, NodeAnim);
		uint32_t NextScalingIndex = ScalingIndex + 1;

		Check(NextScalingIndex < NodeAnim->mNumScalingKeys);

		float t1 = (float)NodeAnim->mScalingKeys[ScalingIndex].mTime;
		float t2 = (float)NodeAnim->mScalingKeys[NextScalingIndex].mTime;

		float DeltaTime = t2 - t1;
		float Factor = (AnimationTimeInTicks - (float)t1) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);

		const aiVector3D& Start = NodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = NodeAnim->mScalingKeys[NextScalingIndex].mValue;

		aiVector3D Delta = End - Start;
		OutScale = Start + Factor * Delta;
	}

	void SkeletalModelImporter::CalculateInterpolatedRotation(aiQuaternion& OutQuat, float AnimationTimeInTicks, const aiNodeAnim* NodeAnim)
	{
		// At least two values are needed to interpolate
		if (NodeAnim->mNumRotationKeys == 1) 
		{
			OutQuat = NodeAnim->mRotationKeys[0].mValue;
			return;
		}

		uint32_t RotationIndex = FindRotation(AnimationTimeInTicks, NodeAnim);
		uint32_t NextRotationIndex = RotationIndex + 1;

		Check(NextRotationIndex < NodeAnim->mNumRotationKeys);

		float t1 = (float)NodeAnim->mRotationKeys[RotationIndex].mTime;
		float t2 = (float)NodeAnim->mRotationKeys[NextRotationIndex].mTime;

		float DeltaTime = t2 - t1;
		float Factor = (AnimationTimeInTicks - t1) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);

		const aiQuaternion& StartRotationQ = NodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = NodeAnim->mRotationKeys[NextRotationIndex].mValue;

		aiQuaternion::Interpolate(OutQuat, StartRotationQ, EndRotationQ, Factor);
		OutQuat.Normalize();
	}

	void SkeletalModelImporter::CalculateInterpolatedLocation(aiVector3D& OutLocation, float AnimationTimeInTicks, const aiNodeAnim* NodeAnim)
	{	
		// At least two values are needed to interpolate
		if (NodeAnim->mNumPositionKeys == 1) 
		{
			OutLocation = NodeAnim->mPositionKeys[0].mValue;
			return;
		}

		uint32_t PositionIndex = FindPosition(AnimationTimeInTicks, NodeAnim);
		uint32_t NextPositionIndex = PositionIndex + 1;

		Check(NextPositionIndex < NodeAnim->mNumPositionKeys);

		float t1 = (float)NodeAnim->mPositionKeys[PositionIndex].mTime;
		float t2 = (float)NodeAnim->mPositionKeys[NextPositionIndex].mTime;

		float DeltaTime = t2 - t1;

		float Factor = (AnimationTimeInTicks - t1) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = NodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = NodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		OutLocation = Start + Factor * Delta;
	}
}