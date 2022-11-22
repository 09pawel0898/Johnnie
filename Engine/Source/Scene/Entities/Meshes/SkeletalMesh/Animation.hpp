#pragma once

#include "Core/CoreMinimal.hpp"
#include "SkeletalMeshHelpers.hpp"

#include <glm/glm.hpp>

struct aiAnimation;

namespace Engine
{
	struct AnimatedNodeData
	{
		std::string Name;
		glm::mat4	Transformation;
		
		std::vector<AnimatedNodeData>	Children;
		uint8_t							ChildrenCount;

		AnimatedNodeData* Parent{ nullptr };
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
		return m_LocalTransform;
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

	class Animation final
	{
		friend class AnimationImporter;

	private:
		std::string_view m_Name = "";

		float	m_Duration = 0.f;
		float	m_TicksPerSecond = 0.f;

		bool	m_bIsValid{ false };

		AnimatedNodeData m_RootNode;
		
		std::vector<AnimatedBoneData> m_Bones;
		std::unordered_map<std::string_view, BoneData> m_BoneInfoMap;
		std::map<std::string, uint32_t> m_BoneNameIndexMap;

	public:
		Animation() = default;
		explicit Animation(std::string_view Name)
			:	m_Name(Name)
		{}

	public:
		bool IsValid(void) const;
		const AnimatedNodeData* GetRootNode(void) const;

		float GetTicksPerSecond(void) const;
		float GetDuration(void) const;
		std::string_view GetName(void) const;
		AnimatedBoneData* FindBone(std::string_view Name);

		std::unordered_map<std::string_view, BoneData>& GetBoneInfoMap(void);
		
		std::map<std::string, uint32_t> const& GetBoneNameToIndexMap(void);

	};

	FORCEINLINE std::string_view Animation::GetName(void) const
	{
		return m_Name;
	}

	FORCEINLINE const AnimatedNodeData* Animation::GetRootNode(void) const
	{
		return &m_RootNode;
	}

	FORCEINLINE bool Animation::IsValid(void) const
	{
		return m_bIsValid;
	}

	FORCEINLINE float Animation::GetTicksPerSecond(void) const
	{
		return (m_TicksPerSecond != 0)? m_TicksPerSecond : 25.f;
	}

	FORCEINLINE float Animation::GetDuration(void) const
	{
		return m_Duration;
	}

	FORCEINLINE std::unordered_map<std::string_view, BoneData>& Animation::GetBoneInfoMap(void)
	{
		return m_BoneInfoMap;
	}
	
	FORCEINLINE std::map<std::string, uint32_t> const& Animation::GetBoneNameToIndexMap(void)
	{
		return m_BoneNameIndexMap;
	}
}