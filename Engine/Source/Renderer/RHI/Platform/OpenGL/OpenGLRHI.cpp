#include "OpenGLRHI.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::RHI
{
	static void OpenGLDebugOutput(GLuint Source, GLuint Type, GLuint Id, GLuint Severity, int Length, const char* Msg, const void* UserParam)
	{
		if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204)
		{
			return;
		}

		switch (Severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         LOG(RHI, Error, Msg);		return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG(RHI, Error, Msg);		return;
			case GL_DEBUG_SEVERITY_LOW:          LOG(RHI, Warning, Msg);	return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(RHI, Trace, Msg);		return;
		}

		switch (Source)
		{
			case GL_DEBUG_SOURCE_API:				LOG(RHI, Error, "Source: API");				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		LOG(RHI, Error, "Source: Window System");	break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:	LOG(RHI, Error, "Source: Shader Compiler"); break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:		LOG(RHI, Error, "Source: Third Party");		break;
			case GL_DEBUG_SOURCE_APPLICATION:		LOG(RHI, Error, "Source: Application");		break;
			case GL_DEBUG_SOURCE_OTHER:				LOG(RHI, Error, "Source: Other");			break;
		}

		switch (Type)
		{
			case GL_DEBUG_TYPE_ERROR:               LOG(RHI, Error, "Type: Error");					break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: LOG(RHI, Error, "Type: Deprecated Behaviour");	break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  LOG(RHI, Error, "Type: Undefined Behaviour");	break;
			case GL_DEBUG_TYPE_PORTABILITY:         LOG(RHI, Error, "Type: Portability");			break;
			case GL_DEBUG_TYPE_PERFORMANCE:         LOG(RHI, Error, "Type: Performance");			break;
			case GL_DEBUG_TYPE_MARKER:              LOG(RHI, Error, "Type: Marker");				break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          LOG(RHI, Error, "Type: Push Group");			break;
			case GL_DEBUG_TYPE_POP_GROUP:           LOG(RHI, Error, "Type: Pop Group");				break;
			case GL_DEBUG_TYPE_OTHER:               LOG(RHI, Error, "Type: Other");					break;
		}
	}

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

#ifndef NDEBUG
		/** Enable debug (OpenGL > 4.3) */
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		/** Enable blending */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/** Enable depth buffer */
		//glEnable(GL_DEPTH_TEST);

		/** Enable draw lines with correct filtering instead of aliased. */
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRHI::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
	{
		glViewport(X, Y, Width, Height);
	}

	void OpenGLRHI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r,color.g,color.b,color.a);
	}

	void OpenGLRHI::Clear(void)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
}