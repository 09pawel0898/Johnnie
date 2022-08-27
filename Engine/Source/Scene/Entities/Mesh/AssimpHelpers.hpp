#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"

#include <assimp/scene.h>

namespace Engine
{
	FORCEINLINE aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHITextureType RHITextureType)
	{
		static std::unordered_map<RHI::RHITextureType, aiTextureType> map =
		{
			{RHI::RHITextureType::Diffuse,	aiTextureType_DIFFUSE	},
			{RHI::RHITextureType::Specular,	aiTextureType_SPECULAR	},
		};
		return map[RHITextureType];
	}
}

