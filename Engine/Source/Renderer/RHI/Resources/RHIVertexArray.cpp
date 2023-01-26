#include "EnginePCH.hpp"

#include "RHIVertexArray.hpp"
#include "Renderer/Renderer.hpp"
#include "../Platform/OpenGL/Resources/OpenGLVertexArray.hpp"
#include "RHIBuffers.hpp"

namespace Engine::RHI
{
	TUniquePtr<RHIVertexArray> RHIVertexArray::Create(void)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return MakeUnique<OpenGLVertexArray>(); break;
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