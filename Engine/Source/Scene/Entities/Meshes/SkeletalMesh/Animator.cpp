#include "EnginePCH.hpp"
#include "Animator.hpp"
#include "SkeletalMeshHelpers.hpp"

namespace Engine
{
    Animator::Animator()
    {
        m_CurrentTimeInTicks = 0.f;
        m_ActiveAnimationName = ID::None;
        
        InitializeLogCategory();

        m_FinalBoneTransformations.resize(g_MaxBonesCount);
    }

    void Animator::OnTick(double DeltaTime)
    {
        m_DeltaTime = DeltaTime;

        if (IsAnimationActive())
        {
            Animation const& Animation = GetActiveAnimation();

            m_CurrentTimeInTicks += Animation.GetTicksPerSecond() * (float)DeltaTime;
            m_CurrentTimeInTicks = fmod(m_CurrentTimeInTicks, Animation.GetDuration()); // loop anim
            
            CalculateBoneTransformations(Animation.GetRootNode(), glm::mat4(1.0f));
        }
    }

    void Animator::InitializeLogCategory(void)
    {
        static bool LoggerInitialized = false;
        if (!LoggerInitialized)
        {
            DEFINE_LOG_CATEGORY(Animator);
            LoggerInitialized = true;
        }
    }

    float Animator::SecondsToTicks(float AnimationTimeInSeconds) const
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

    Animation const& Animator::GetActiveAnimation(void) const
    {
        Check(m_ActiveAnimationName != ID::None);

        const auto Anim = m_Animations.find(m_ActiveAnimationName);
        return Anim->second;
    }

    void CalculateBoneTransformations(const AnimatedNodeData* Node, glm::mat4 const& ParentTransform)
    {
        //std::string NodeName    = Node->Name;
        //glm::mat4 nodeTransform = Node->Transformation;
        //
        //AnimatedBoneData* Bone = m_CurrentAnimation->FindBone(NodeName);
        //
        //if (Bone)
        //{
        //    Bone->Update(m_CurrentTime);
        //    nodeTransform = Bone->GetLocalTransform();
        //}
        //
        //glm::mat4 globalTransformation = parentTransform * nodeTransform;
        //
        //auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        //if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        //{
        //    int index = boneInfoMap[nodeName].id;
        //    glm::mat4 offset = boneInfoMap[nodeName].offset;
        //    m_FinalBoneMatrices[index] = globalTransformation * offset;
        //}
        //
        //for (int i = 0; i < node->childrenCount; i++)
        //    CalculateBoneTransform(&node->children[i], globalTransformation);
    }

    std::vector<std::string> Animator::GetAvailableAnimationsNames(void) const
    {
        return std::vector<std::string>();
    }

    uint8_t Animator::GetAvailableAnimationsCount(void) const
    {
        return uint8_t();
    }
    
    bool Animator::SetActiveAnimationName(std::string const& AnimationName)
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

    void Animator::Pause(void)
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
    
    void Animator::Resume(void)
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

    void Animator::SetAnimationTime(float TimeInSeconds, bool PauseAnimation)
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

    void Animator::SetLoop(bool Loop)
    {
        m_bLoopAnimation = Loop;
    }

    void Animator::ClearAnimations(void)
    {
        m_ActiveAnimationName = ID::None;

        m_Animations.clear();
    }
}