#pragma once

#include "Core/CoreMinimal.hpp"
#include "SkeletalMeshHelpers.hpp"

#include <glm/glm.hpp>

struct aiAnimation;

namespace Engine
{
	class AnimationBoneKeyFrames
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
		std::vector<PositionKey>	m_PositionKeyFrames;
		std::vector<RotationKey>	m_RotationKeyFrames;
		std::vector<ScaleKey>		m_ScaleKeyFrames;

		std::string	m_BoneName;
		uint16_t	m_BoneIndex;

		glm::mat4	m_LocalTransform;

	public:
		AnimationBoneKeyFrames(std::string BoneName, uint16_t BoneIndex, const aiNodeAnim* AiChannel);

	private:
		void ParsePositionKeys(const aiNodeAnim* AiChannel);
		void ParseRotationKeys(const aiNodeAnim* AiChannel);
		void ParseScaleKeys(const aiNodeAnim* AiChannel);

	public:
		void UpdateLocalTransform(float AnimationTimeInTicks);

	public:
		std::string const& GetBoneName(void) const;
		int32_t GetBoneIndex(void) const;
		glm::mat4 const& GetLocalTransform(void) const;

	private:
		double GetScaleFactor(double LastTimeStamp, double NextTimeStamp, float AnimationTimeInTicks) const;

		int32_t GetPositionIndex(float AnimationTimeInTicks) const;
		int32_t GetRotationIndex(float AnimationTimeInTicks) const;
		int32_t GetScaleIndex(float AnimationTimeInTicks) const;

		glm::mat4 InterpolatePosition(float AnimationTimeInTicks) const;
		glm::mat4 InterpolateRotation(float AnimationTimeInTicks) const;
		glm::mat4 InterpolateScaling(float AnimationTimeInTicks) const;
	};

	FORCEINLINE glm::mat4 const& AnimationBoneKeyFrames::GetLocalTransform(void) const
	{
		return m_LocalTransform;
	}
	FORCEINLINE std::string const& AnimationBoneKeyFrames::GetBoneName(void) const
	{
		return m_BoneName;
	}
	FORCEINLINE int32_t AnimationBoneKeyFrames::GetBoneIndex(void) const
	{
		return m_BoneIndex;
	}

	class Animation final
	{
		friend class AnimationImporter;

	private:
		std::string_view m_AnimationName = "";

		float	m_Duration = 0.f;
		float	m_TicksPerSecond = 0.f;

		std::vector<AnimationBoneKeyFrames>				m_BoneKeyFrames;
		std::unordered_map<std::string_view, BoneData>	m_BoneInfoMap;
		std::map<std::string, uint32_t>					m_BoneNameIndexMap;

	public:
		static inline constexpr uint8_t s_MaxBonesCount = 200;

	public:
		Animation() = default;
		explicit Animation(std::string_view Name)
			:	m_AnimationName(Name)
		{}

	public:
		float GetTicksPerSecond(void) const;
		float GetDuration(void) const;
		std::string_view GetName(void) const;

	public:
		AnimationBoneKeyFrames* FindBoneKeyFrames(std::string_view BoneName);
		std::unordered_map<std::string_view, BoneData>& GetBoneInfoMap(void);
		std::map<std::string, uint32_t> const& GetBoneNameToIndexMap(void) const;
	};

	FORCEINLINE std::string_view Animation::GetName(void) const
	{
		return m_AnimationName;
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
	
	FORCEINLINE std::map<std::string, uint32_t> const& Animation::GetBoneNameToIndexMap(void) const
	{
		return m_BoneNameIndexMap;
	}
}