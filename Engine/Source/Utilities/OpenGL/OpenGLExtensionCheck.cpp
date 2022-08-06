#include "OpenGLExtensionCheck.hpp"

/** TO BE REMOVED (no explicit glad includes despite RHI) */
#include <glad/glad.h>

#include <cstring>
#include "OpenGLCallCheck.hpp"

namespace Engine::Utility::OpenGL
{
	bool IsExtensionSupported(const char* Extension)
	{
        GLint numExtensions = 0;
        GLCall(glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions));

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