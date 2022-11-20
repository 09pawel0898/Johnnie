#pragma once

#include "Core/CoreMinimal.hpp"

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct aiNode;
struct aiNodeAnim;

namespace Engine
{
	class VertexBoneInfluenceData
	{
	private:
		static constexpr inline int8_t s_MaxBonesPerVertex = 4;

	public:
		uint16_t Index = 0;

		/** Contains IDs of bones that invluence this vertex */
		int32_t	BoneIDs[s_MaxBonesPerVertex] = { -1 };

		/** Contains influence weights for associated bone IDs in BoneIDs array */
		float	Weights[s_MaxBonesPerVertex] = { 0.f };

	public:
		VertexBoneInfluenceData() = default;

		void AddBoneData(uint32_t GlobalVertexID, uint32_t BoneID, float Weight);
	};

	struct BoneData
	{
		uint16_t	Index = Index::None;
		glm::mat4	OffsetMatrix;
		glm::mat4	FinalTransformation;

		BoneData() = default;
		explicit BoneData(glm::mat4 const& OffsetMatrix);
		explicit BoneData(glm::mat4&& OffsetMatrix) noexcept;
	};

	struct NodeData
	{
		const aiNode*	Node = nullptr;
		bool			IsRequired = false;

		NodeData() = default;
		NodeData(const aiNode* Node)
			:	Node(Node)
		{}
	};

	struct ModelSkeletonData
	{
		/** Contains information about which bones influence each vertex. Size is equal to vertices count in loaded model */
		std::vector<VertexBoneInfluenceData>	VertexBoneInfluenceData;

		/** Contains global index of first vertex for each mesh in model */
		std::vector<uint32_t>					MeshBaseVertex;

		/** Maps imported by assimp bone names to indexes */
		std::map<std::string, uint32_t>			BoneNameIndexMap;

		/** */
		std::vector<BoneData>					BonesData;

		std::map<std::string, NodeData>			RequiredNodes;
	};

	class AnimatedBoneData
	{
	public:
		struct PositionKey
		{
			glm::vec3 Position;
			double TimeStamp;
		};

		struct RotationKey
		{
			glm::quat Rotation;
			double TimeStamp;
		};

		struct ScaleKey
		{
			glm::vec3 Scale;
			double TimeStamp;
		};
	private:
		std::vector<PositionKey>	m_Positions;
		std::vector<RotationKey>	m_Rotations;
		std::vector<ScaleKey>		m_Scales;

		std::string	m_Name;
		uint16_t	m_Index;
		glm::mat4	m_LocalTransform;
		
	public:
		AnimatedBoneData(std::string Name, uint16_t Index, const aiNodeAnim* Channel);

	private:
		void ParsePositionKeys(const aiNodeAnim* Channel);
		void ParseRotationKeys(const aiNodeAnim* Channel);
		void ParseScaleKeys(const aiNodeAnim* Channel);

    public:
        void Update(float AnimationTimeInTicks);

		glm::mat4 GetLocalTransform(void) const;
		std::string const& GetBoneName(void) const;
		int32_t GetBoneIndex(void) const;

        int32_t GetPositionIndex(float AnimationTimeInTicks) const;

		int32_t GetRotationIndex(float AnimationTimeInTicks) const;
		
		int32_t GetScaleIndex(float AnimationTimeInTicks) const;

    private:
		double GetScaleFactor(double LastTimeStamp, double NextTimeStamp, float AnimationTimeInTicks) const;
		
		glm::mat4 InterpolatePosition(float AnimationTimeInTicks) const;
		glm::mat4 InterpolateRotation(float AnimationTimeInTicks) const;
		glm::mat4 InterpolateScaling(float AnimationTimeInTicks) const;
	};

	FORCEINLINE glm::mat4 AnimatedBoneData::GetLocalTransform(void) const
	{
		return glm::mat4();
	}
	FORCEINLINE std::string const& AnimatedBoneData::GetBoneName(void) const
	{
		return m_Name;
	}
	FORCEINLINE int32_t AnimatedBoneData::GetBoneIndex(void) const
	{
		return m_Index;
	}

	static inline constexpr uint8_t g_MaxBonesCount = 200;
}