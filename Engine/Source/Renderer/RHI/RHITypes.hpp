#pragma once

#include "Core/CoreMinimal.hpp"

namespace Engine::RHI
{
	enum class RHIElementType : uint8_t
	{
		None = 0, Int, Int2, Int3, Int4, Bool, Float, Float2, Float3, Float4, Mat3, Mat4
	};

	size_t GetRHIElementTypeSize(RHIElementType ElementType);
}