#include "EnginePCH.hpp"
#include "Animation.hpp"

#include <assimp/scene.h>

namespace Engine
{
	AnimatedBoneData* Animation::FindBone(std::string const& Name)
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