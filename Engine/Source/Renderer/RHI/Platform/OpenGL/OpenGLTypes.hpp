#pragma once

#include "Renderer/RHI/RHITypes.hpp"

namespace Engine::RHI
{
	uint32_t RHIElementTypeToOpenGL(RHIElementType ElementType);
	bool RHIIsIntegerElement(RHIElementType ElementType);
	uint32_t RHIInternalTextureFormatToOpenGL(RHIInternalTextureFormat InternalFormat);
}