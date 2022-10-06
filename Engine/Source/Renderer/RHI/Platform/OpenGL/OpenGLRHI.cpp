#include "EnginePCH.hpp"

#include "OpenGLRHI.hpp"
#include "Resources/OpenGLBuffers.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::RHI
{
	/** OpenGL Error Handler */
	static void OpenGLDebugOutput(GLuint Source, GLuint Type, GLuint Id, GLuint Severity, int Length, const char* Msg, const void* UserParam)
	{
		if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204)
		{
			return;
		}

		auto generateInfoHeader = [Source,Type]() -> std::string
		{
			std::string sourceInfo, typeInfo;

			switch (Source)
			{
				case GL_DEBUG_SOURCE_API:				sourceInfo = "Source: API";				break;
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		sourceInfo = "Source: Window System";	break;
				case GL_DEBUG_SOURCE_SHADER_COMPILER:	sourceInfo = "Source: Shader Compiler"; break;
				case GL_DEBUG_SOURCE_THIRD_PARTY:		sourceInfo = "Source: Third Party";		break;
				case GL_DEBUG_SOURCE_APPLICATION:		sourceInfo = "Source: Application";		break;
				case GL_DEBUG_SOURCE_OTHER:				sourceInfo = "Source: Other";			break;
			}

			switch (Type)
			{
				case GL_DEBUG_TYPE_ERROR:               typeInfo = "Type: Error";				break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeInfo = "Type: Deprecated Behaviour";break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeInfo = "Type: Undefined Behaviour";	break;
				case GL_DEBUG_TYPE_PORTABILITY:         typeInfo = "Type: Portability";			break;
				case GL_DEBUG_TYPE_PERFORMANCE:         typeInfo = "Type: Performance";			break;
				case GL_DEBUG_TYPE_MARKER:              typeInfo = "Type: Marker";				break;
				case GL_DEBUG_TYPE_PUSH_GROUP:          typeInfo = "Type: Push Group";			break;
				case GL_DEBUG_TYPE_POP_GROUP:           typeInfo = "Type: Pop Group";			break;
				case GL_DEBUG_TYPE_OTHER:               typeInfo = "Type: Other";				break;
			}
			return "(OpenGL) (" + typeInfo + ") (" + sourceInfo + ") ";
		};

		switch (Severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         LOG(RHI, Error, "{0} {1}", generateInfoHeader(), Msg);	break;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG(RHI, Error, Msg);		break;
			case GL_DEBUG_SEVERITY_LOW:          LOG(RHI, Warning, Msg);	break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(RHI, Trace, Msg);		break;
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
		glEnable(GL_DEPTH_TEST);

		/** Enable draw lines with correct filtering instead of aliased */
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRHI::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
	{
		glViewport(X, Y, Width, Height);
	}

	void OpenGLRHI::SetWireframeMode(bool Enabled)
	{
		if(Enabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		m_bWireframeMode = Enabled;
	}

	void OpenGLRHI::SetClearColor(const glm::vec4& Color)
	{
		glClearColor(Color.r,Color.g,Color.b,Color.a);
	}

	void OpenGLRHI::Clear(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRHI::DrawLines(std::shared_ptr<RHIVertexArray> const& VertexArray, uint32_t VertexCount)
	{
		VertexArray->Bind();
		glDrawArrays(GL_LINES, 0, VertexCount);
	}

	void OpenGLRHI::DrawIndexed(std::shared_ptr<RHIVertexArray> const& VertexArray, uint32_t IndexCount)
	{
		VertexArray->Bind();
		uint32_t count = IndexCount ? IndexCount : VertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
}