#include "EnginePCH.hpp"
#include "Animation.hpp"

#include "Log/Log.hpp"
#include "Utilities/GLMUtility.hpp"

#include <assimp/scene.h>

namespace Engine
{
    AnimatedBoneData::AnimatedBoneData(std::string Name, uint16_t Index, const aiNodeAnim* Channel)
        : m_Name(MoveTemp(Name)),
        m_Index(Index),
        m_LocalTransform(1.0f)
    {
#ifdef DEBUG_MODEL_IMPORTER
        LOG(Core, Trace, "AnimatedBoneData [{0}]", m_Name);
#endif
        ParsePositionKeys(Channel);
        ParseRotationKeys(Channel);
        ParseScaleKeys(Channel);
    }
    void AnimatedBoneData::ParsePositionKeys(const aiNodeAnim* Channel)
    {
        for (uint16_t idx = 0; idx < Channel->mNumPositionKeys; ++idx)
        {
            aiVector3D const& aiPosition = Channel->mPositionKeys[idx].mValue;

            double TimeStamp = Channel->mPositionKeys[idx].mTime;
            PositionKey PositionKey;
            PositionKey.Position = Utility::AiVec3ToGlmVec3(aiPosition);
            PositionKey.TimeStamp = TimeStamp;

            m_Positions.emplace_back(MoveTemp(PositionKey));
        }
    }

    void AnimatedBoneData::ParseRotationKeys(const aiNodeAnim* Channel)
    {
        for (uint16_t idx = 0; idx < Channel->mNumRotationKeys; idx++)
        {
            aiQuaternion const& aiQuaternion = Channel->mRotationKeys[idx].mValue;

            double TimeStamp = Channel->mRotationKeys[idx].mTime;
            RotationKey RotationKey;
            RotationKey.Rotation = Utility::AiQuatToGlmQuat(aiQuaternion);
            RotationKey.TimeStamp = TimeStamp;

            m_Rotations.emplace_back(MoveTemp(RotationKey));
        }
    }

    void AnimatedBoneData::ParseScaleKeys(const aiNodeAnim* Channel)
    {
        for (uint16_t idx = 0; idx < Channel->mNumScalingKeys; idx++)
        {
            aiVector3D const& Scale = Channel->mScalingKeys[idx].mValue;

            double TimeStamp = Channel->mScalingKeys[idx].mTime;
            ScaleKey ScaleKey;
            ScaleKey.Scale = Utility::AiVec3ToGlmVec3(Scale);
            ScaleKey.TimeStamp = TimeStamp;

            m_Scales.emplace_back(MoveTemp(ScaleKey));
        }
    }

    void AnimatedBoneData::Update(float AnimationTime)
    {
        glm::mat4 Translation = InterpolatePosition(AnimationTime);
        glm::mat4 Rotation = InterpolateRotation(AnimationTime);
        glm::mat4 Scale = InterpolateScaling(AnimationTime);
        m_LocalTransform = Translation * Rotation * Scale;
    }

    int32_t AnimatedBoneData::GetPositionIndex(float AnimationTimeInTicks) const
    {
        for (uint16_t idx = 0; idx < m_Positions.size() - 1; idx++)
        {
            if (AnimationTimeInTicks < m_Positions[idx + 1].TimeStamp)
            {
                return idx;
            }
        }
        CheckMsg(false, "Position index not found");

        return Index::None;
    }

    int32_t AnimatedBoneData::GetRotationIndex(float AnimationTimeInTicks) const
    {
        for (uint16_t idx = 0; idx < m_Rotations.size() - 1; idx++)
        {
            if (AnimationTimeInTicks < m_Rotations[idx + 1].TimeStamp)
            {
                return idx;
            }
        }
        CheckMsg(false, "Rotation index not found");

        return Index::None;
    }

    int32_t AnimatedBoneData::GetScaleIndex(float AnimationTimeInTicks) const
    {
        for (uint16_t idx = 0; idx < m_Scales.size() - 1; idx++)
        {
            if (AnimationTimeInTicks < m_Scales[idx + 1].TimeStamp)
            {
                return idx;
            }
        }
        CheckMsg(false, "Scale index not found");

        return Index::None;
    }

    double AnimatedBoneData::GetScaleFactor(double LastTimeStamp, double NextTimeStamp, float AnimationTimeInTicks) const
    {
        double MidWayLength = (double)AnimationTimeInTicks - LastTimeStamp;
        double FramesDiff = NextTimeStamp - LastTimeStamp;
        double ScaleFactor = MidWayLength / FramesDiff;

        return ScaleFactor;
    }

    glm::mat4 AnimatedBoneData::InterpolatePosition(float AnimationTimeInTicks) const
    {
        if (m_Positions.size() == 1)
        {
            return glm::translate(glm::mat4(1.0f), m_Positions[0].Position);
        }

        int p0Index = GetPositionIndex(AnimationTimeInTicks);
        int p1Index = p0Index + 1;

        double ScaleFactor = GetScaleFactor(m_Positions[p0Index].TimeStamp,
            m_Positions[p1Index].TimeStamp, AnimationTimeInTicks);
        glm::vec3 FinalPosition = glm::mix(m_Positions[p0Index].Position,
            m_Positions[p1Index].Position, ScaleFactor);

        return glm::translate(glm::mat4(1.0f), FinalPosition);
    }

    glm::mat4 AnimatedBoneData::InterpolateRotation(float AnimationTimeInTicks) const
    {
        if (m_Rotations.size() == 1)
        {
            auto rotation = glm::normalize(m_Rotations[0].Rotation);
            return glm::mat4_cast(rotation);
        }

        int p0Index = GetRotationIndex(AnimationTimeInTicks);
        int p1Index = p0Index + 1;

        double ScaleFactor = GetScaleFactor(m_Rotations[p0Index].TimeStamp,
            m_Rotations[p1Index].TimeStamp, AnimationTimeInTicks);

        glm::quat FinalRotation = glm::slerp(m_Rotations[p0Index].Rotation,
            m_Rotations[p1Index].Rotation, (float)ScaleFactor);
        FinalRotation = glm::normalize(FinalRotation);

        return glm::mat4_cast(FinalRotation);
    }

    glm::mat4 AnimatedBoneData::InterpolateScaling(float AnimationTimeInTicks) const
    {
        if (m_Scales.size() == 1)
        {
            return glm::scale(glm::mat4(1.0f), m_Scales[0].Scale);
        }

        int p0Index = GetScaleIndex(AnimationTimeInTicks);
        int p1Index = p0Index + 1;

        double ScaleFactor = GetScaleFactor(m_Scales[p0Index].TimeStamp,
            m_Scales[p1Index].TimeStamp, AnimationTimeInTicks);

        glm::vec3 FinalScale = glm::mix(m_Scales[p0Index].Scale, m_Scales[p1Index].Scale, ScaleFactor);

        return glm::scale(glm::mat4(1.0f), FinalScale);
    }

	AnimatedBoneData* Animation::FindBone(std::string_view Name)
	{
		auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
			[&](const AnimatedBoneData& Bone)
			{
				return Bone.GetBoneName() == Name;
			}
		);
		return (iter == m_Bones.end()) ? nullptr : &(*iter);
	}
}