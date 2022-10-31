#include "EnginePCH.hpp"

#include <glad/glad.h>
#include "OpenGLHelpers.hpp"

namespace Utility::OpenGL
{
	void BindTexture(uint32_t RendererID, uint32_t Slot)
	{
		glActiveTexture(GL_TEXTURE0 + Slot);
		glBindTexture(GL_TEXTURE_2D, RendererID);
	}
}