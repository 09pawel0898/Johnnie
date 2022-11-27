#include "EnginePCH.hpp"
#include "Animator.hpp"
#include "SkeletalMeshHelpers.hpp"
#include "SkeletalMesh.hpp"
#include "Utilities/GLMUtility.hpp"

namespace Engine
{
    OAnimator::OAnimator()
    {
        m_CurrentTimeInTicks = 0.f;
        m_ActiveAnimationName = ID::None;
        
        InitializeLogCategory();

        m_FinalBoneTransformations.resize(Animation::s_MaxBonesCount);
    }

    void OAnimator::OnTick(double DeltaTime)
    {
        m_DeltaTime = DeltaTime;

        TSharedPtr<ASkeletalMesh> SkeletalMesh = m_AnimatedSkeletalMesh.lock();
        
        if (!SkeletalMesh)
        {
            m_AnimatedSkeletalMesh.reset();
            SetTickEnabled(false);
            return;
        }

        if (IsAnimationActive() && !m_bPaused)
        {
            if (SkeletalMesh->IsActorReadyToDraw())
            {
                UpdateSkeletalMeshBoneTransformations(SkeletalMesh);
            }
        }
    }

    void OAnimator::InitializeLogCategory(void)
    {
        static bool LoggerInitialized = false;
        if (!LoggerInitialized)
        {
            DEFINE_LOG_CATEGORY(Animator);
            LoggerInitialized = true;
        }
    }

    float OAnimator::SecondsToTicks(float AnimationTimeInSeconds)
    {
        if (IsAnimationActive())
        {
            const auto Anim = m_Animations.find(m_ActiveAnimationName);

            Animation const& Animation = Anim->second;

            const float TicksPerSecond = Animation.GetTicksPerSecond();
            const float TimeInTicks = AnimationTimeInSeconds * TicksPerSecond;

            if(m_bLoopAnimation)
            {
                return fmod(TimeInTicks, Animation.GetDuration()); // loop animation
            }
            else if (TimeInTicks > Animation.GetDuration())
            {
                m_CurrentTimeInTicks = 0.f;
                m_bPaused = true;
            }
        }
        return 0.f;
    }

    Animation const& OAnimator::GetActiveAnimation(void) const
    {
        Check(m_ActiveAnimationName != ID::None);

        const auto Anim = m_Animations.find(m_ActiveAnimationName);
        return Anim->second;
    }

    void OAnimator::UpdateSkeletalMeshBoneTransformations(TSharedPtr<ASkeletalMesh>& SkeletalMesh)
    {
        Animation const& Animation = GetActiveAnimation();

        m_CurrentTimeInTicks += Animation.GetTicksPerSecond() * (float)m_DeltaTime;
        
        if (m_bLoopAnimation)
        {
            m_CurrentTimeInTicks = fmod(m_CurrentTimeInTicks, Animation.GetDuration());
        }
        else if (m_CurrentTimeInTicks > Animation.GetDuration())
        {
            m_CurrentTimeInTicks = 0.f;
            m_bPaused = true;
        }

        NodeData* RootBone = nullptr;
        SkeletalMesh->GetSkeleton().GetRootBone(&RootBone);
        
        Check(RootBone);

        CalculateBoneTransformations(RootBone, glm::mat4(1.0f));
        SkeletalMesh->SetCurrentBoneTransformations(m_FinalBoneTransformations);
    }

    void OAnimator::CalculateBoneTransformations(const NodeData* Node, glm::mat4 const& ParentTransform)
    {
        TSharedPtr<ASkeletalMesh> SkeletalMesh = m_AnimatedSkeletalMesh.lock();
        Check(SkeletalMesh);

        const std::string NodeName  = Node->Name;
        glm::mat4 NodeTransform     = Node->Transformation;
        
        Animation& CurrentAnimation = m_Animations[m_ActiveAnimationName];
        AnimationBoneKeyFrames* Bone = CurrentAnimation.FindBoneKeyFrames(NodeName);
        
        if (Bone)
        {
            Bone->UpdateLocalTransform(m_CurrentTimeInTicks);
            NodeTransform = Bone->GetLocalTransform();
        }
        glm::mat4 GlobalTransformation = ParentTransform * NodeTransform;
        
        std::vector<BoneData> const& BonesData = SkeletalMesh->GetSkeleton().GetBonesData();
        
        auto& BoneNameToIndexMap = CurrentAnimation.GetBoneNameToIndexMap();
        if (BoneNameToIndexMap.find(NodeName) != BoneNameToIndexMap.end())
        {
            uint32_t BoneIndex = BoneNameToIndexMap.at(NodeName);

            glm::mat4 const& OffsetMatrix = BonesData[BoneIndex].OffsetMatrix;

            m_FinalBoneTransformations[BoneIndex] = SkeletalMesh->GetSkeleton().GetGlobalInverseTransform() * GlobalTransformation * OffsetMatrix;
        }

        for (uint16_t idx = 0; idx < Node->ChildrenCount; idx++)
        {
            std::string ChildName = Node->Children[idx].Name;

            const NodeData* ChildNode = SkeletalMesh->GetSkeleton().FindNodeByName(ChildName);
            CheckMsg(ChildNode != nullptr, "Child node couldn't be found in the hierarchy.");

            if (ChildNode->IsRequired)
            {
                CalculateBoneTransformations(&Node->Children[idx], GlobalTransformation);
            }
        }
    }

    void OAnimator::AsyncImportSingleAnimationFromFile(std::string const& FilePath, bool ActivateOnLoad)
    {
        TSharedPtr<ASkeletalMesh> SkeletalMesh = m_AnimatedSkeletalMesh.lock();

        if (SkeletalMesh == nullptr)
        {
            LOG(Animator, Warning, "Can't import animation, no valid skeletal mesh is loaded.");
            return;
        }

        if (!m_AnimationImporter.IsBusy())
        {
            SkeletalMesh->GetSkeleton();
            m_bPendingActivateFirstOnLoad = ActivateOnLoad;
            m_AnimationImporter.AsyncImportFirstAnimation(SkeletalMesh, FilePath, OnAnimationAsyncLoadingFinishedDelegate::CreateRaw(this, &OAnimator::OnSingleAnimationLoadedFromFile));
        }
        else
        {
            LOG(Animator, Warning, "Loading canceled. Animation importer is busy.");
        }
    }

    void OAnimator::AsyncImportAllAnimationsFromFile(std::string const& FilePath, bool ActivateFirstOnLoad)
    {
        TSharedPtr<ASkeletalMesh> SkeletalMesh = m_AnimatedSkeletalMesh.lock();
        
        if (SkeletalMesh == nullptr)
        {
            LOG(Animator, Warning, "Can't import animations, no valid skeletal mesh is loaded.");
            return;
        }

        if (!m_AnimationImporter.IsBusy())
        {
            m_bPendingActivateFirstOnLoad = ActivateFirstOnLoad;
            m_AnimationImporter.AsyncImportAllAnimations(SkeletalMesh, FilePath, OnAnimationsAsyncLoadingFinishedDelegate::CreateRaw(this, &OAnimator::OnAnimationsLoadedFromFile));
        } 
        else
        {
            LOG(Animator, Warning, "Loading canceled. Animation importer is busy.");
        }
    }

    std::vector<std::string_view> OAnimator::GetAvailableAnimationsNames(void) const
    {
        std::vector<std::string_view> Names;

        for (auto const& [Name, Animation] : m_Animations)
        {
            Names.emplace_back(Name);
        }
        
        return Names;
    }

    uint8_t OAnimator::GetAvailableAnimationsCount(void) const
    {
        return (uint8_t)m_Animations.size();
    }
    
    bool OAnimator::SetActiveAnimationName(std::string const& AnimationName)
    {
        bool FoundAnimation = m_Animations.find(AnimationName) != m_Animations.cend();

        if (FoundAnimation)
        {
            m_ActiveAnimationName = AnimationName;
            m_CurrentTimeInTicks = 0.f;

            return true;
        }
        LOG(Animator, Warning, "Can't activate requested animation, there's no such animation registered.");
        return false;
    }

    float OAnimator::GetActiveAnimationDurationInSeconds(void) const
    {
        if (!IsAnimationActive())
        {
            return 0.f;
        }

        auto FoundAnimation = m_Animations.find(m_ActiveAnimationName);
        if (FoundAnimation != m_Animations.cend())
        {
            return FoundAnimation->second.GetDuration() / FoundAnimation->second.GetTicksPerSecond();
        }
        return 0.f;
    }

    float OAnimator::GetCurrentAnimationTimeInSeconds(void) const
    {
        if (!IsAnimationActive())
        {
            return 0.f;
        }

        auto FoundAnimation = m_Animations.find(m_ActiveAnimationName);
        if (FoundAnimation != m_Animations.cend())
        {
            return m_CurrentTimeInTicks / FoundAnimation->second.GetTicksPerSecond();
        }
        return 0.f;
    }

    void OAnimator::Pause(void)
    {
        if (IsAnimationActive())
        {
            m_bPaused = true;
        }
        else
        {
            LOG(Animator, Warning, "There's no active animation, nothing to pause.");
        }
    }
    
    void OAnimator::Resume(void)
    {
        if (IsAnimationActive())
        {
            m_bPaused = false;
        }
        else
        {
            LOG(Animator, Warning, "There's no active animation, nothing to resume.");
        }
    }

    void OAnimator::SetAnimationTime(float TimeInSeconds, bool PauseAnimation)
    {
        if (IsAnimationActive())
        {
            m_CurrentTimeInTicks = SecondsToTicks(TimeInSeconds);
            
            if (PauseAnimation)
            {
                TSharedPtr<ASkeletalMesh> SkeletalMesh = m_AnimatedSkeletalMesh.lock();
                Check(SkeletalMesh);

                UpdateSkeletalMeshBoneTransformations(SkeletalMesh);
                Pause();
            }
        }
        else
        {
            LOG(Animator, Warning, "Can't set animation time because there's no active animation.");
        }
    }

    void OAnimator::SetLoop(bool Loop)
    {
        m_bLoopAnimation = Loop;
    }

    void OAnimator::ClearAnimations(void)
    {
        m_ActiveAnimationName = ID::None;

        m_Animations.clear();
    }

    void OAnimator::OnAnimationsLoadedFromFile(std::vector<Animation>& Animations)
    {
        //Check(Animations.size() > 0);
        //
        //std::string FirstName;
        //
        //for (uint16_t idx = 0 ; idx < Animations.size() ;idx++)
        //{
        //    const std::string Name = Animations[idx].GetName().data();
        //
        //    if (idx == 0)
        //    {
        //        FirstName = "Animation_" + Name;
        //    }
        //
        //    m_Animations["Animation_" + Name] = Animations[idx];
        //}
        //
        //if (m_bPendingActivateFirstOnLoad)
        //{
        //    SetActiveAnimationName(m_Animations[FirstName].GetName());
        //}
    }

    void OAnimator::OnSingleAnimationLoadedFromFile(Animation& Animation)
    {
        static uint16_t AnimationID = 0;

        const std::string Name = "Animation_" + std::to_string(AnimationID);

        m_Animations[Name] = Animation;
        
        if (m_bPendingActivateFirstOnLoad)
        {
            SetActiveAnimationName(Name);
        }

        OnAnimationLoaded.Broadcast((uint32_t)(m_Animations.size()-1));

        AnimationID++;
    }

    bool OAnimator::HasValidSkeletalMesh(void) const
    {
        if (TSharedPtr<ASkeletalMesh> SkeletalMesh = m_AnimatedSkeletalMesh.lock())
        {
            return SkeletalMesh->IsActorReadyToDraw();
        }
        return false;
    }
}