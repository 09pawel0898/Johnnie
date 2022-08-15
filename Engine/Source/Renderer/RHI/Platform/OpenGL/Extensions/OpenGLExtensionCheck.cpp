#include "EnginePCH.hpp"

#include "OpenGLExtensionCheck.hpp"
#include <glad/glad.h>

namespace Engine::Utility::OpenGL
{
	bool IsExtensionSupported(const char* Extension)
	{
        GLint numExtensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

        for (int i = 0; i < numExtensions; i++) 
        {
            const char* extensionNameToCheck = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
            if (strstr(extensionNameToCheck, Extension))
            {
                return true;
            }
        }
        return false;
    }
}