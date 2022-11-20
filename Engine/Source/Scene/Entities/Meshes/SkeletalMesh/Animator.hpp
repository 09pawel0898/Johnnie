#pragma once

#include "Core/CoreMinimal.hpp"
#include "Animation.hpp"
#include "Utilities/Delegates.hpp"
#include "Scene/Entities/Primitives/Tickable.hpp"
#include "Log/Log.hpp"

namespace Engine
{
	class Animator final : public Tickable
	{
	private:
		using AnimationID = std::string;

		std::unordered_map<AnimationID, Animation>	m_Animations;
		std::string	m_ActiveAnimationName = ID::None;

		bool		m_bIsAnyAnimationActive{ false };
		bool		m_bLoopAnimation{ false };
		bool		m_bPaused{ false };

		float		m_CurrentTimeInTicks;
		double		m_DeltaTime;

		std::vector<glm::mat4> m_FinalBoneTransformations;

	public:
		DECLARE_MULTICAST_DELEGATE(OnAnimationStartedPlaying,	std::string const&);
		DECLARE_MULTICAST_DELEGATE(OnAnimationFinishedPlaying,	std::string const&);
		DECLARE_MULTICAST_DELEGATE(OnAnimationPausedPlaying,	std::string const&);
		DECLARE_MULTICAST_DELEGATE(OnAnimationResumedPlaying,	std::string const&);
		
		OnAnimationStartedPlaying	OnAnimationStarted;
		OnAnimationFinishedPlaying	OnAnimationFinished;

		OnAnimationPausedPlaying	OnAnimationPaused;
		OnAnimationResumedPlaying	OnAnimationResumed;

	public:
		Animator();

	private:
		void InitializeLogCategory(void);

		float SecondsToTicks(float AnimationTimeInSeconds) const;
		Animation const& GetActiveAnimation(void) const;

		void CalculateBoneTransformations(const AnimatedNodeData* Node, glm::mat4 const& ParentTransform);

	public:
		virtual void OnTick(double DeltaTime) override;

		template <typename TAnimation>
			requires BaseOf<Animation,TAnimation>
		void AddAnimation(TAnimation&& Animation)
		{
			m_Animations.insert(Forward<TAnimation>(Animation));
		}

	public:
		bool IsAnimationActive(void) const;
		std::string const& GetActiveAnimationName(void) const;

		bool SetActiveAnimationName(std::string const& AnimationName);
		std::vector<std::string> GetAvailableAnimationsNames(void) const;
		uint8_t GetAvailableAnimationsCount(void) const;
		
		void Pause(void);
		void Resume(void);
		void SetAnimationTime(float TimeInSeconds, bool PauseAnimation);
		void SetLoop(bool Loop);

		void ClearAnimations(void);

		std::vector<glm::mat4> const& GetBoneTransformations(void) const;
	};

	FORCEINLINE bool Animator::IsAnimationActive(void) const
	{
		return m_ActiveAnimationName != ID::None;
	}

	FORCEINLINE std::string const& Animator::GetActiveAnimationName(void) const
	{
		return m_ActiveAnimationName;
	}

	FORCEINLINE std::vector<glm::mat4> const& Animator::GetBoneTransformations(void) const
	{
		return m_FinalBoneTransformations;
	}
}