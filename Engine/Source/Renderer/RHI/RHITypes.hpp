#pragma once

#include "Core/CoreMinimal.hpp"
#include <glm/glm.hpp>

namespace Engine::RHI
{
	enum class RHIElementType : uint8_t
	{
		None = 0, Int, Int2, Int3, Int4, Bool, Float, Float2, Float3, Float4, Mat3, Mat4
	};

	size_t GetRHIElementTypeSize(RHIElementType ElementType);

	struct RHIVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexUV;
		glm::vec3 Tangent;
	};

	enum class RHITextureType : uint8_t
	{
		BasicTexture2D,
		Diffuse,
		Specular,
		Normal
	};

	std::string GetUniformNameByTextureType(RHITextureType TextureType);
}