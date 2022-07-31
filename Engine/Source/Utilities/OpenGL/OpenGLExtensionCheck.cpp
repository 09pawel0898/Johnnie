#include "OpenGLExtensionCheck.hpp"

#include <glad/glad.h>
#include <cstring>

#include "OpenGLCallCheck.hpp"

namespace Engine::Utilities::OpenGL
{
	bool IsExtensionSupported(const char* Extension)
	{
        GLint numExtensions = 0;
        GLCall(glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions));

        for (int i = 0; i < numExtensions; i++) 
        {
            const char* extensionNameToCheck = (const char*)(glGetStringi(GL_EXTENSIONS, i));
            if (strstr(extensionNameToCheck, Extension))
            {
                return true;
            }
        }
        return false;
    }
}