#include "EnginePCH.hpp"
#include "Animation.hpp"

#include "Log/Log.hpp"
#include "Utilities/GLMUtility.hpp"

#include <assimp/scene.h>

namespace Engine
{
    AnimationBoneKeyFrames::AnimationBoneKeyFrames(std::string BoneName, uint16_t BoneIndex, const aiNodeAnim* AiChannel)
        :   m_BoneName(MoveTemp(BoneName)),
            m_BoneIndex(BoneIndex),
            m_LocalTransform(1.0f)
    {
        ParsePositionKeys(AiChannel);
        ParseRotationKeys(AiChannel);
        ParseScaleKeys(AiChannel);
    }

    void AnimationBoneKeyFrames::ParsePositionKeys(const aiNodeAnim* AiChannel)
    {
        for (uint16_t idx = 0; idx < AiChannel->mNumPositionKeys; ++idx)
        {
            aiVector3D const& aiPosition = AiChannel->mPositionKeys[idx].mValue;

            double TimeStamp = AiChannel->mPositionKeys[idx].mTime;
            PositionKey PositionKey;
            PositionKey.Position = Utility::AiVec3ToGlmVec3(aiPosition);
            PositionKey.TimeStamp = TimeStamp;

            m_PositionKeyFrames.emplace_back(MoveTemp(PositionKey));
        }
    }

    void AnimationBoneKeyFrames::ParseRotationKeys(const aiNodeAnim* AiChannel)
    {
        for (uint16_t idx = 0; idx < AiChannel->mNumRotationKeys; idx++)
        {
            aiQuaternion const& aiQuaternion = AiChannel->mRotationKeys[idx].mValue;

            double TimeStamp = AiChannel->mRotationKeys[idx].mTime;
            RotationKey RotationKey;
            RotationKey.Rotation = Utility::AiQuatToGlmQuat(aiQuaternion);
            RotationKey.TimeStamp = TimeStamp;

            m_RotationKeyFrames.emplace_back(MoveTemp(RotationKey));
        }
    }

    void AnimationBoneKeyFrames::ParseScaleKeys(const aiNodeAnim* AiChannel)
    {
        for (uint16_t idx = 0; idx < AiChannel->mNumScalingKeys; idx++)
        {
            aiVector3D const& Scale = AiChannel->mScalingKeys[idx].mValue;

            double TimeStamp = AiChannel->mScalingKeys[idx].mTime;
            ScaleKey ScaleKey;
            ScaleKey.Scale = Utility::AiVec3ToGlmVec3(Scale);
            ScaleKey.TimeStamp = TimeStamp;

            m_ScaleKeyFrames.emplace_back(MoveTemp(ScaleKey));
        }
    }

    void AnimationBoneKeyFrames::UpdateLocalTransform(float AnimationTime)
    {
        glm::mat4 Translation   = InterpolatePosition(AnimationTime);
        glm::mat4 Rotation      = InterpolateRotation(AnimationTime);
        glm::mat4 Scale         = InterpolateScaling(AnimationTime);

        m_LocalTransform = Translation * Rotation * Scale;
    }

    int32_t AnimationBoneKeyFrames::GetPositionIndex(float AnimationTimeInTicks) const
    {
        for (uint16_t idx = 0; idx < m_PositionKeyFrames.size() - 1; idx++)
        {
            if (AnimationTimeInTicks < m_PositionKeyFrames[idx + 1].TimeStamp)
            {
                return idx;
            }
        }
        CheckMsg(false, "Position index not found");

        return Index::None;
    }

    int32_t AnimationBoneKeyFrames::GetRotationIndex(float AnimationTimeInTicks) const
    {
        for (uint16_t idx = 0; idx < m_RotationKeyFrames.size() - 1; idx++)
        {
            if (AnimationTimeInTicks < m_RotationKeyFrames[idx + 1].TimeStamp)
            {
                return idx;
            }
        }
        CheckMsg(false, "Rotation index not found");

        return Index::None;
    }

    int32_t AnimationBoneKeyFrames::GetScaleIndex(float AnimationTimeInTicks) const
    {
        for (uint16_t idx = 0; idx < m_ScaleKeyFrames.size() - 1; idx++)
        {
            if (AnimationTimeInTicks < m_ScaleKeyFrames[idx + 1].TimeStamp)
            {
                return idx;
            }
        }
        CheckMsg(false, "Scale index not found");

        return Index::None;
    }

    double AnimationBoneKeyFrames::GetScaleFactor(double LastTimeStamp, double NextTimeStamp, float AnimationTimeInTicks) const
    {
        double MidWayLength = (double)AnimationTimeInTicks - LastTimeStamp;
        double FramesDiff = NextTimeStamp - LastTimeStamp;
        double ScaleFactor = MidWayLength / FramesDiff;

        return ScaleFactor;
    }

    glm::mat4 AnimationBoneKeyFrames::InterpolatePosition(float AnimationTimeInTicks) const
    {
        if (m_PositionKeyFrames.size() == 1)
        {
            return glm::translate(glm::mat4(1.0f), m_PositionKeyFrames[0].Position);
        }

        int p0Index = GetPositionIndex(AnimationTimeInTicks);
        int p1Index = p0Index + 1;

        double ScaleFactor = GetScaleFactor(m_PositionKeyFrames[p0Index].TimeStamp,
                                            m_PositionKeyFrames[p1Index].TimeStamp, 
                                            AnimationTimeInTicks);
        
        glm::vec3 FinalPosition = glm::mix( m_PositionKeyFrames[p0Index].Position,
                                            m_PositionKeyFrames[p1Index].Position, 
                                            ScaleFactor);

        return glm::translate(glm::mat4(1.0f), FinalPosition);
    }

    glm::mat4 AnimationBoneKeyFrames::InterpolateRotation(float AnimationTimeInTicks) const
    {
        if (m_RotationKeyFrames.size() == 1)
        {
            auto rotation = glm::normalize(m_RotationKeyFrames[0].Rotation);
            return glm::mat4_cast(rotation);
        }

        int p0Index = GetRotationIndex(AnimationTimeInTicks);
        int p1Index = p0Index + 1;

        double ScaleFactor = GetScaleFactor(m_RotationKeyFrames[p0Index].TimeStamp,
                                            m_RotationKeyFrames[p1Index].TimeStamp, 
                                            AnimationTimeInTicks);

        glm::quat FinalRotation = glm::slerp(m_RotationKeyFrames[p0Index].Rotation,
                                             m_RotationKeyFrames[p1Index].Rotation, 
                                             (float)ScaleFactor);
        
        FinalRotation = glm::normalize(FinalRotation);

        return glm::mat4_cast(FinalRotation);
    }

    glm::mat4 AnimationBoneKeyFrames::InterpolateScaling(float AnimationTimeInTicks) const
    {
        if (m_ScaleKeyFrames.size() == 1)
        {
            return glm::scale(glm::mat4(1.0f), m_ScaleKeyFrames[0].Scale);
        }

        int p0Index = GetScaleIndex(AnimationTimeInTicks);
        int p1Index = p0Index + 1;

        double ScaleFactor = GetScaleFactor(m_ScaleKeyFrames[p0Index].TimeStamp,
                                            m_ScaleKeyFrames[p1Index].TimeStamp, 
                                            AnimationTimeInTicks);

        glm::vec3 FinalScale = glm::mix(m_ScaleKeyFrames[p0Index].Scale, m_ScaleKeyFrames[p1Index].Scale, ScaleFactor);

        return glm::scale(glm::mat4(1.0f), FinalScale);
    }

    AnimationBoneKeyFrames* Animation::FindBoneKeyFrames(std::string_view BoneName)
    {
        auto FoundBoneKeyFrames = std::find_if(m_BoneKeyFrames.begin(), m_BoneKeyFrames.end(),
            [&](const AnimationBoneKeyFrames& BoneKeyFrames)
            {
                return BoneKeyFrames.GetBoneName() == BoneName;
            }
        );
        return (FoundBoneKeyFrames == m_BoneKeyFrames.end()) ? nullptr : &(*FoundBoneKeyFrames);
    }
}