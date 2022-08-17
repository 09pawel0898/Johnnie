#include "EnginePCH.hpp"

#include "OpenGLTypes.hpp"

#include <glad/glad.h>

namespace Engine::RHI
{
	uint32_t RHIElementTypeToOpenGL(RHIElementType ElementType)
	{
		static const std::unordered_map<RHIElementType, uint32_t> RHIElementSizes =
		{
			{RHIElementType::Int,		GL_INT},
			{RHIElementType::Int2,		GL_INT},
			{RHIElementType::Int3,		GL_INT},
			{RHIElementType::Int3,		GL_INT},
			{RHIElementType::Bool,		GL_BOOL},
			{RHIElementType::Float,		GL_FLOAT},
			{RHIElementType::Float2,	GL_FLOAT},
			{RHIElementType::Float3,	GL_FLOAT},
			{RHIElementType::Float4,	GL_FLOAT},
			{RHIElementType::Mat3,		GL_FLOAT},
			{RHIElementType::Mat4,		GL_FLOAT},
		};
		return RHIElementSizes.at(ElementType);
	}
}
