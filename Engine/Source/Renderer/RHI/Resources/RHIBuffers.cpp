#include "EnginePCH.hpp"

#include "Renderer/Renderer.hpp"
#include "RHIBuffers.hpp"

#include "../Platform/OpenGL/Resources/OpenGLBuffers.hpp"

namespace Engine::RHI
{
	size_t RHIGetVertexBufferElementTypeSize(RHIElementType VBOElementType)
	{
		return GetRHIElementTypeSize(VBOElementType);
	}

	void RHIVertexBufferLayout::CalculateOffsetAndStride(void)
	{
		size_t offset = m_Stride = 0;

		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	RHIVertexBufferLayout::RHIVertexBufferLayout(std::initializer_list<RHIVertexBufferElement> Elements)
		:	m_Elements(Elements)
	{
		CalculateOffsetAndStride();
	}

	TUniquePtr<RHIIndexBuffer> RHIIndexBuffer::Create(uint32_t* Indices, uint32_t Count)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return MakeUnique<OpenGLIndexBuffer>(Indices, Count); break;
		}
		return nullptr;
	}

	TUniquePtr<RHIIndexBuffer> RHIIndexBuffer::Create(std::vector<uint32_t> const& Indices)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return MakeUnique<OpenGLIndexBuffer>(Indices); break;
		}
		return nullptr;
	}

	TUniquePtr<RHIVertexBuffer> RHIVertexBuffer::Create(uint32_t Size)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return MakeUnique<OpenGLVertexBuffer>(Size); break;
		}
		return nullptr;
	}

	TUniquePtr<RHIVertexBuffer> RHIVertexBuffer::Create(const void* Vertices, uint32_t Size)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return MakeUnique<OpenGLVertexBuffer>(Vertices, Size); break;
		}
		return nullptr;
	}

	TUniquePtr<RHIVertexBuffer> RHIVertexBuffer::Create(std::vector<RHIVertex> const& Vertices)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return MakeUnique<OpenGLVertexBuffer>(Vertices); break;
		}
		return nullptr;
	}
	
}
