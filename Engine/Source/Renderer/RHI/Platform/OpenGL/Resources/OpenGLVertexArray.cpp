#include "EnginePCH.hpp"

#include "OpenGLVertexArray.hpp"
#include "OpenGLBuffers.hpp"

#include <glad/glad.h>

namespace Engine::RHI
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<RHIVertexBuffer> VertexBuffer)
	{
		Check(VertexBuffer->GetLayout()->GetElements().size() > 0);
		
		Bind();
		VertexBuffer->Bind();



		m_VertexBuffers.emplace_back(std::move(VertexBuffer));
	}

	void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<RHIIndexBuffer> IndexBuffer)
	{
		Check(IndexBuffer->GetCount() > 0);

		m_IndexBuffer = std::move(IndexBuffer);

		Bind();
		m_IndexBuffer->Bind();
	}
}