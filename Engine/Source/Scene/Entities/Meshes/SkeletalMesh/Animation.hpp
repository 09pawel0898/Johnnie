#pragma once

#include "Core/CoreMinimal.hpp"
#include "SkeletalMeshHelpers.hpp"

#include <glm/glm.hpp>

struct aiAnimation;

namespace Engine
{
	class AnimatedBoneData;

	struct AnimatedNodeData
	{
		std::string Name;
		glm::mat4	Transformation;
		
		uint8_t							ChildrenCount;
		std::vector<AnimatedNodeData>	Children;
	};

	class Animation final
	{
		friend class AnimationImporter;

	private:
		float	m_Duration;
		float	m_TicksPerSecond;

		bool	m_bIsValid{ false };

		AnimatedNodeData m_RootNode;
		
		std::vector<AnimatedBoneData> m_Bones;
		std::unordered_map<std::string, BoneData> m_BoneInfoMap;
	public:
		Animation() = default;
		
	public:
		bool IsValid(void) const;
		const AnimatedNodeData* GetRootNode(void) const;

		float GetTicksPerSecond(void) const;
		float GetDuration(void) const;

		AnimatedBoneData* FindBone(std::string const& Name);
	};

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
}