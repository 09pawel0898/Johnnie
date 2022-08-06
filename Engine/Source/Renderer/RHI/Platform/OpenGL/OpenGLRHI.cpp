#include "OpenGLRHI.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Engine::RHI
{
	void OpenGLRHI::Init(void)
	{
#ifdef PLATFORM_WINDOWS
		auto result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#else
		CheckMsg(false, "Couldn't initialize OpenGLRHI context for this platform.");
#endif

		GLint majorVersion, minorVersion;
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		if (minorVersion < OpenGL_MINOR_VERSION && majorVersion < OpenGL_MAJOR_VERSION)
		{
			LOG(RHI,Error, "OpenGL {0}.{1} is required", OpenGL_MAJOR_VERSION, OpenGL_MINOR_VERSION);
			Check(false);
		}
		else
		{
			LOG(RHI, Info, "OpenGLRHI context initialized. Version : {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		}
	}
}