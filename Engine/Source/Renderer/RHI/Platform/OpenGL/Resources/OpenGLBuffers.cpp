#include "EnginePCH.hpp"
#include "OpenGLBuffers.hpp"

#include <glad/glad.h>

namespace Engine::RHI
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* Indices, uint32_t Count)
		:	RHIIndexBuffer(Count)
	{
		glGenBuffers(1, &m_ID);

		/*	GL_ARRAY_BUFFER instead of GL_ELEMENT_ARRAY_BUFFER because of a need of VAO to
		 *	be valid during GL_ELEMENT_ARRAY_BUFFER binding operation */

		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(uint32_t), Indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t> const& Indices)
		: RHIIndexBuffer((uint32_t)Indices.size())
	{
		glGenBuffers(1, &m_ID);

		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(uint32_t), Indices.data(), GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t Size)
	{
		UNIMPLEMENTED
		//glGenBuffers(1, &m_ID);
		//glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		//glBufferData(GL_ARRAY_BUFFER, Size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* Vertices, uint32_t Size)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, Size, Vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<RHIVertex> const& Vertices)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(RHIVertex), Vertices.data(), GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::Unbind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* Data, uint32_t Size)
	{
		UNIMPLEMENTED
	}

	const void* OpenGLVertexBuffer::GetData(void) const
	{
		UNIMPLEMENTED
		return nullptr;
	}

	std::unique_ptr<RHIVertexBufferLayout> const& OpenGLVertexBuffer::GetLayout() const
	{
		return m_VertexBufferLayout;
	}

	void OpenGLVertexBuffer::SetLayout(std::unique_ptr<RHIVertexBufferLayout> Layout)
	{
		m_VertexBufferLayout = std::move(Layout);
	}
}