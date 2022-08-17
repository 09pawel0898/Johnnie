#include "EnginePCH.hpp"

#include "RHITypes.hpp"

namespace Engine::RHI
{
	size_t Engine::RHI::GetRHIElementTypeSize(RHIElementType ElementType)
	{
		static const std::unordered_map<RHIElementType, size_t> RHIElementSizes =
		{
			{RHIElementType::Int,		4},
			{RHIElementType::Int2,		4 * 2},
			{RHIElementType::Int3,		4 * 3},
			{RHIElementType::Int3,		4 * 3},
			{RHIElementType::Bool,		1},
			{RHIElementType::Float,		4},
			{RHIElementType::Float2,	4 * 2},
			{RHIElementType::Float3,	4 * 3},
			{RHIElementType::Float4,	4 * 4},
			{RHIElementType::Mat3,		4 * 3 * 3},
			{RHIElementType::Mat4,		4 * 4 * 4},
		};
		return RHIElementSizes.at(ElementType);
	}
}
