#include "EnginePCH.hpp"

#include "../../Renderer.hpp"
#include "RHIBuffers.hpp"

#include "../Platform/OpenGL/Resources/OpenGLBuffers.hpp"

namespace Engine::RHI
{
	size_t RHIGetVertexBufferElementTypeSize(RHIVertexBufferElementType VBOElementType)
	{
		static const std::unordered_map<RHIVertexBufferElementType, size_t> sizes =
		{
			{RHIVertexBufferElementType::Int,		4},
			{RHIVertexBufferElementType::Int2,		4*2},
			{RHIVertexBufferElementType::Int3,		4*3},
			{RHIVertexBufferElementType::Int3,		4*3},
			{RHIVertexBufferElementType::Bool,		1},
			{RHIVertexBufferElementType::Float,		4},
			{RHIVertexBufferElementType::Float2,	4*2},
			{RHIVertexBufferElementType::Float3,	4*3},
			{RHIVertexBufferElementType::Float4,	4*4},
			{RHIVertexBufferElementType::Mat3,		4*3*3},
			{RHIVertexBufferElementType::Mat4,		4*4*4},
		};
		return sizes.at(VBOElementType);
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
		switch (Renderer::GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLIndexBuffer>(Indices, Count); break;
		}
		return nullptr;
	}

	std::unique_ptr<RHIVertexBuffer> RHIVertexBuffer::Create(uint32_t Size)
	{
		switch (Renderer::GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLVertexBuffer>(Size); break;
		}
		return nullptr;
	}

	std::unique_ptr<RHIVertexBuffer> RHIVertexBuffer::Create(void const* Vertices, uint32_t Size)
	{
		switch (Renderer::GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLVertexBuffer>(Vertices, Size); break;
		}
		return nullptr;
	}
}
