#include "EnginePCH.hpp"

#include "OpenGLVertexArray.hpp"
#include "OpenGLBuffers.hpp"
#include "../OpenGLTypes.hpp"

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

		uint8_t currentAttribIndex = 0;
		
		auto const& layout = VertexBuffer->GetLayout();

		for (auto element = layout->begin(); element != layout->end(); element++)
		{
			switch (element->VBOElementType)
			{
				using enum RHIElementType;

			case Float3:
				glEnableVertexAttribArray(currentAttribIndex);
				glVertexAttribPointer(currentAttribIndex, 3, RHIElementTypeToOpenGL(element->VBOElementType),
					element->bNormalized ? GL_TRUE : GL_FALSE, (int32_t)layout->GetStride(), (void const*)element->Offset);
				currentAttribIndex++;
				break;

			case Float4:
				glEnableVertexAttribArray(currentAttribIndex);
				glVertexAttribPointer(currentAttribIndex, 4, RHIElementTypeToOpenGL(element->VBOElementType),
					element->bNormalized ? GL_TRUE : GL_FALSE, (int32_t)layout->GetStride(), (void const*)element->Offset);
				currentAttribIndex++;
				break;

			default:
				CheckMsg(false, "Unsupported Element for OpenGL VertexBufferLayout!");
				break;
			}
		}
		
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