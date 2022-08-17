#include "EnginePCH.hpp"

#include "RHIVertexArray.hpp"

#include "Renderer/Renderer.hpp"
#include "../Platform/OpenGL/Resources/OpenGLVertexArray.hpp"

namespace Engine::RHI
{
	std::unique_ptr<RHIVertexArray> RHIVertexArray::Create(void)
	{
		switch (Renderer::GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLVertexArray>(); break;
		}
		return nullptr;
	}
}