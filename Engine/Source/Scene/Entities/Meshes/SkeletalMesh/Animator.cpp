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
        
        if (SkeletalMesh != nullptr && IsAnimationActive())
        {
            if (SkeletalMesh->IsActorReadyToDraw())
            {
                Animation const& Animation = GetActiveAnimation();

                m_CurrentTimeInTicks += Animation.GetTicksPerSecond() * (float)DeltaTime;
                m_CurrentTimeInTicks = fmod(m_CurrentTimeInTicks, Animation.GetDuration()); // loop anim
                
                NodeData* RootBone = nullptr;

                SkeletalModelImporter* SkelModelImporter = Cast<SkeletalModelImporter>(SkeletalMesh->GetImporter().get());
                if (SkelModelImporter)
                {
                    SkeletalModelImporter::FindRootBone(
                        &SkelModelImporter->GetSkeletonData().RootNode, &RootBone, Animation.GetBoneNameToIndexMap());
                }
                Check(RootBone);

                CalculateBoneTransformations(RootBone, glm::mat4(1.0f));

                SkeletalMesh->SetCurrentBoneTransformations(m_FinalBoneTransformations);
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

    float OAnimator::SecondsToTicks(float AnimationTimeInSeconds) const
    {
        if (IsAnimationActive())
        {
            const auto Anim = m_Animations.find(m_ActiveAnimationName);

            Animation const& Animation = Anim->second;

            const float TicksPerSecond = Animation.GetTicksPerSecond();
            const float TimeInTicks = AnimationTimeInSeconds * TicksPerSecond;

            return fmod(TimeInTicks, Animation.GetDuration()); // loop animation
        }
        return 0.f;
    }

    Animation const& OAnimator::GetActiveAnimation(void) const
    {
        Check(m_ActiveAnimationName != ID::None);

        const auto Anim = m_Animations.find(m_ActiveAnimationName);
        return Anim->second;
    }

    void OAnimator::CalculateBoneTransformations(const NodeData* Node, glm::mat4 const& ParentTransform)
    {
        const std::string NodeName  = Node->Name;
        glm::mat4 NodeTransform     = Node->Transformation;
        
        Animation& CurrentAnimation = m_Animations[m_ActiveAnimationName];

        AnimationBoneKeyFrames* Bone = CurrentAnimation.FindBoneKeyFrames(NodeName);
        
        if (Bone)
        {
            Bone->UpdateLocalTransform(m_CurrentTimeInTicks);
            NodeTransform = Bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = ParentTransform * NodeTransform;
        
        //
        std::vector<BoneData> BD;
        uint32_t ID;
        TSharedPtr<ASkeletalMesh> SkeletalMesh = m_AnimatedSkeletalMesh.lock();
        if (SkeletalMesh)
        {
            SkeletalModelImporter* SkelModelImporter = Cast<SkeletalModelImporter>(SkeletalMesh->GetImporter().get());
            if (SkelModelImporter)
            {
                

                BD = SkelModelImporter->GetSkeletonData().BonesData;
        
        //
                auto& BoneNameToIndexMap = CurrentAnimation.GetBoneNameToIndexMap();
        
                if (BoneNameToIndexMap.find(NodeName) != BoneNameToIndexMap.end())
                {
                    std::string s = NodeName;
                    ID = BoneNameToIndexMap.at(s);

                    glm::mat4 const& OffsetMatrix = BD[ID].OffsetMatrix;

                    m_FinalBoneTransformations[ID] = SkelModelImporter->GetGlobalInverseTransform() * globalTransformation * OffsetMatrix;
                }
                for (uint16_t idx = 0; idx < Node->ChildrenCount; idx++)
                {
                    std::string ChildName = Node->Children[idx].Name;

                    NodeData* ChildNode = SkelModelImporter->GetSkeletonData().FindNodeByName(ChildName);
                    CheckMsg(ChildNode != nullptr, "Child node couldn't be found in the hierarchy.");

                    if (ChildNode->IsRequired)
                    {
                        CalculateBoneTransformations(&Node->Children[idx], globalTransformation);
                    }
                }
            }
        }
    }

    void OAnimator::AsyncImportSingleAnimationFromFile(std::string_view FilePath, bool ActivateOnLoad)
    {
        if (!m_AnimationImporter.IsBusy())
        {
            m_bPendingActivateFirstOnLoad = ActivateOnLoad;
            m_AnimationImporter.m_SkeletalMesh = m_AnimatedSkeletalMesh; // TEMP
            m_AnimationImporter.AsyncImportFirstAnimation(FilePath, OnAnimationAsyncLoadingFinishedDelegate::CreateRaw(this, &OAnimator::OnSingleAnimationLoadedFromFile));
        }
        else
        {
            LOG(Animator, Warning, "Loading canceled. Animation importer is busy.");
        }
    }

    void OAnimator::AsyncImportAllAnimationsFromFile(std::string_view FilePath, bool ActivateFirstOnLoad)
    {
        if (!m_AnimationImporter.IsBusy())
        {
            m_bPendingActivateFirstOnLoad = ActivateFirstOnLoad;
            m_AnimationImporter.m_SkeletalMesh = m_AnimatedSkeletalMesh; // TEMP
            m_AnimationImporter.AsyncImportAllAnimations(FilePath, OnAnimationsAsyncLoadingFinishedDelegate::CreateRaw(this, &OAnimator::OnAnimationsLoadedFromFile));
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
        return uint8_t();
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

        AnimationID++;
    }
}