#include "EnginePCH.hpp"

#include "RHIVertexArray.hpp"

#include "Renderer/Renderer.hpp"
#include "../Platform/OpenGL/Resources/OpenGLVertexArray.hpp"
#include "RHIBuffers.hpp"

namespace Engine::RHI
{
	std::unique_ptr<RHIVertexArray> RHIVertexArray::Create(void)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLVertexArray>(); break;
		}
		return nullptr;
	}

	uint32_t RHIVertexArray::GetVertexCount(void)
	{
		uint32_t numVertices = 0;
		for (auto const& buffer : m_VertexBuffers)
		{
			numVertices += buffer->GetVerticesCount();
		}
		return numVertices;
	}
}