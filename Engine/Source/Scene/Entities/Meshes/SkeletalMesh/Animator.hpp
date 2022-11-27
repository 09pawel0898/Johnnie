#pragma once

#include "Core/CoreMinimal.hpp"
#include "Animation.hpp"
#include "Utilities/Delegates.hpp"
#include "Scene/Entities/Primitives/Object.hpp"

#include "../AssetImporter.hpp"
#include "Log/Log.hpp"

namespace Engine
{
	class ASkeletalMesh;

	class OAnimator final : public Object
	{
	private:
		TWeakPtr<ASkeletalMesh> m_AnimatedSkeletalMesh;

	private:
		using AnimationID = std::string;

		AnimationImporter m_AnimationImporter;
		bool m_bPendingActivateFirstOnLoad{ false };

	private:
		std::unordered_map<AnimationID, Animation>	m_Animations;
		std::string	m_ActiveAnimationName = ID::None;

		bool		m_bIsAnyAnimationActive{ false };
		bool		m_bLoopAnimation{ true };
		bool		m_bPaused{ false };

		float		m_CurrentTimeInTicks;
		double		m_DeltaTime;

		std::vector<glm::mat4> m_FinalBoneTransformations;

	public:
		DECLARE_MULTICAST_DELEGATE(OnNewAnimationLoaded, uint32_t);
		DECLARE_MULTICAST_DELEGATE(OnAnimationStartedPlaying,	std::string const&);
		DECLARE_MULTICAST_DELEGATE(OnAnimationFinishedPlaying,	std::string const&);
		DECLARE_MULTICAST_DELEGATE(OnAnimationPausedPlaying,	std::string const&);
		DECLARE_MULTICAST_DELEGATE(OnAnimationResumedPlaying,	std::string const&);
		
		OnNewAnimationLoaded		OnAnimationLoaded;

		OnAnimationStartedPlaying	OnAnimationStarted;
		OnAnimationFinishedPlaying	OnAnimationFinished;

		OnAnimationPausedPlaying	OnAnimationPaused;
		OnAnimationResumedPlaying	OnAnimationResumed;

	public:
		OAnimator();

	private:
		void InitializeLogCategory(void);

		float SecondsToTicks(float AnimationTimeInSeconds);
		
		Animation const& GetActiveAnimation(void) const;

		void CalculateBoneTransformations(const NodeData* Node, glm::mat4 const& ParentTransform);
		void UpdateSkeletalMeshBoneTransformations(TSharedPtr<ASkeletalMesh>& SkeletalMesh);

	public:
		virtual void OnTick(double DeltaTime) override;

		void AsyncImportSingleAnimationFromFile(std::string const& FilePath, bool ActivateOnLoad);
		void AsyncImportAllAnimationsFromFile(std::string const& FilePath, bool ActivateFirstOnLoad);

		void SetSkeletalMesh(TSharedPtr<ASkeletalMesh> SkeletalMesh);
		bool HasValidSkeletalMesh(void) const;

	public:
		bool IsAnimationActive(void) const;
		std::string const& GetActiveAnimationName(void) const;

		bool SetActiveAnimationName(std::string const& AnimationName);
		std::vector<std::string_view> GetAvailableAnimationsNames(void) const;
		uint8_t GetAvailableAnimationsCount(void) const;
		
		float GetActiveAnimationDurationInSeconds(void) const;
		float GetCurrentAnimationTimeInSeconds(void) const;

		void Pause(void);
		bool IsPaused(void) const;
		void Resume(void);
		void SetAnimationTime(float TimeInSeconds, bool PauseAnimation);
		void SetLoop(bool Loop);
		bool IsLooping(void) const;

		void ClearAnimations(void);

		std::vector<glm::mat4> const& GetBoneTransformations(void) const;

	private:
		void OnAnimationsLoadedFromFile(std::vector<Animation>& Animations);
		void OnSingleAnimationLoadedFromFile(Animation& Animation);
	};

	FORCEINLINE bool OAnimator::IsPaused(void) const
	{
		return m_bPaused;
	}
	
	FORCEINLINE bool OAnimator::IsLooping(void) const
	{
		return m_bLoopAnimation;
	}

	FORCEINLINE bool OAnimator::IsAnimationActive(void) const
	{
		return m_ActiveAnimationName != ID::None;
	}

	FORCEINLINE std::string const& OAnimator::GetActiveAnimationName(void) const
	{
		return m_ActiveAnimationName;
	}

	FORCEINLINE std::vector<glm::mat4> const& OAnimator::GetBoneTransformations(void) const
	{
		return m_FinalBoneTransformations;
	}

	FORCEINLINE void OAnimator::SetSkeletalMesh(TSharedPtr<ASkeletalMesh> SkeletalMesh)
	{
		m_AnimatedSkeletalMesh = MoveTemp(SkeletalMesh);
		SetTickEnabled(true);
	}
}