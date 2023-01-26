#pragma once

#include "Core/CoreMinimal.hpp"
#include <glm/glm.hpp>

namespace Engine::RHI
{
	enum class RHIElementType : uint8_t
	{
		None = 0, Int, Int2, Int3, Int4, Int10, Bool, Float, Float2, Float3, Float4, Float10, Mat3, Mat4
	};

	size_t GetRHIElementTypeSize(RHIElementType ElementType);

	struct RHIVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexUV;
		glm::vec3 Tangent;
	};
	
	struct RHISkinnedVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexUV;
		glm::vec3 Tangent;

		int32_t	BoneIDs[4] = { -1 };
		float	Weights[4] = { 0.f };

		explicit RHISkinnedVertex(RHIVertex const& Vertex);

		RHISkinnedVertex& operator=(RHIVertex const& Rhs);
	};

	enum class RHIInternalTextureFormat : uint8_t
	{
		R = 0,
		RGB,
		RGBA,
		BGRA
	};

	enum class RHIMapTextureType : uint8_t
	{
		BasicTexture2D = 0,
		DiffuseMap,
		SpecularMap,
		NormalMap
	};

	std::string GetUniformNameByTextureType(RHIMapTextureType TextureType);
}