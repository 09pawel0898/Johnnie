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
			{RHIElementType::Int4,		GL_INT},
			{RHIElementType::Int10,		GL_INT},
			{RHIElementType::Bool,		GL_BOOL},
			{RHIElementType::Float,		GL_FLOAT},
			{RHIElementType::Float2,	GL_FLOAT},
			{RHIElementType::Float3,	GL_FLOAT},
			{RHIElementType::Float4,	GL_FLOAT},
			{RHIElementType::Float10,	GL_FLOAT},
			{RHIElementType::Mat3,		GL_FLOAT},
			{RHIElementType::Mat4,		GL_FLOAT},
		};
		return RHIElementSizes.at(ElementType);
	}

	bool RHIIsIntegerElement(RHIElementType ElementType)
	{

		if (	ElementType == RHIElementType::Int
			||	ElementType == RHIElementType::Int2
			||	ElementType == RHIElementType::Int3
			||	ElementType == RHIElementType::Int4
			||  ElementType == RHIElementType::Int10)
		{
			return true;
		}
		return false;
	}

	uint32_t RHIInternalTextureFormatToOpenGL(RHIInternalTextureFormat InternalFormat)
	{
		static const std::unordered_map<RHIInternalTextureFormat, uint32_t> RHIInternalFormats =
		{
			{RHIInternalTextureFormat::R,		GL_RED_INTEGER},
			{RHIInternalTextureFormat::RGB,		GL_RGB},
			{RHIInternalTextureFormat::RGBA,	GL_RGBA},
			{RHIInternalTextureFormat::RGBA,	GL_RGBA},
			{RHIInternalTextureFormat::BGRA,	GL_BGRA}
		};
		return RHIInternalFormats.at(InternalFormat);
	}
}
