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

	void OpenGLVertexArray::AddVertexBuffer(TSharedPtr<RHIVertexBuffer> VertexBuffer)
	{
		Check(VertexBuffer->GetLayout()->GetElements().size() > 0);
		
		Bind();
		VertexBuffer->Bind();

		uint8_t currentAttribIndex = 0;
		auto const& layout = VertexBuffer->GetLayout();

		auto add1DVertexAttribArray = [&currentAttribIndex](auto const& Layout, auto const& Element, uint8_t Size)
		{
			glEnableVertexAttribArray(currentAttribIndex);

			if (RHIIsIntegerElement(Element->VBOElementType))
			{
				glVertexAttribIPointer(currentAttribIndex, Size, RHIElementTypeToOpenGL(Element->VBOElementType),
					(int32_t)Layout->GetStride(), (const void*)Element->Offset);
			}
			else
			{
				glVertexAttribPointer(currentAttribIndex, Size, RHIElementTypeToOpenGL(Element->VBOElementType),
					Element->bNormalized ? GL_TRUE : GL_FALSE, (int32_t)Layout->GetStride(), (const void*)Element->Offset);
			}
			currentAttribIndex++;
		};

		for (auto element = layout->begin(); element != layout->end(); element++)
		{
			switch (element->VBOElementType)
			{
				using enum RHIElementType;

				case Float2:	add1DVertexAttribArray(layout, element, 2);	break;
				case Float3:	add1DVertexAttribArray(layout, element, 3);	break;
				case Float4:	add1DVertexAttribArray(layout, element, 4);	break;
				case Float10:	add1DVertexAttribArray(layout, element, 10);break;
				case Int10:		add1DVertexAttribArray(layout, element, 10);break;
				case Int4:		add1DVertexAttribArray(layout, element, 4);break;

				default:
					CheckMsg(false, "Unsupported Element for OpenGL VertexBufferLayout!");
					break;
			}
		}
		
		m_VertexBuffers.emplace_back(MoveTemp(VertexBuffer));
	}

	void OpenGLVertexArray::SetIndexBuffer(TSharedPtr<RHIIndexBuffer> IndexBuffer)
	{
		Check(IndexBuffer->GetCount() > 0);

		m_IndexBuffer = MoveTemp(IndexBuffer);

		Bind();
		m_IndexBuffer->Bind();
	}
}