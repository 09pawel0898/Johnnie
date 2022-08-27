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

	std::unique_ptr<RHIIndexBuffer> RHIIndexBuffer::Create(uint32_t* Indices, uint32_t Count)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLIndexBuffer>(Indices, Count); break;
		}
		return nullptr;
	}

	std::unique_ptr<RHIIndexBuffer> RHIIndexBuffer::Create(std::vector<uint32_t> const& Indices)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLIndexBuffer>(Indices); break;
		}
		return nullptr;
	}

	std::unique_ptr<RHIVertexBuffer> RHIVertexBuffer::Create(uint32_t Size)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLVertexBuffer>(Size); break;
		}
		return nullptr;
	}

	std::unique_ptr<RHIVertexBuffer> RHIVertexBuffer::Create(const void* Vertices, uint32_t Size)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLVertexBuffer>(Vertices, Size); break;
		}
		return nullptr;
	}

	std::unique_ptr<RHIVertexBuffer> RHIVertexBuffer::Create(std::vector<RHIVertex> const& Vertices)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLVertexBuffer>(Vertices); break;
		}
		return nullptr;
	}
	
}
