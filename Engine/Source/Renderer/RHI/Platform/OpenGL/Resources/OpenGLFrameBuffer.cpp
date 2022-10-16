#include "EnginePCH.hpp"

#include "OpenGLFrameBuffer.hpp"
#include <glad/glad.h>

namespace Engine::RHI
{
	namespace RHIUtility
	{

	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(RHIFrameBufferSpecification&& Specification)
		:	RHIFrameBuffer(std::move(Specification))
	{
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
	}

	void OpenGLFrameBuffer::Bind(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	void OpenGLFrameBuffer::Unbind(void)
	{
		UNIMPLEMENTED;
	}
	
	void OpenGLFrameBuffer::Invalidate(void)
	{
		if(m_ID != 0)
		{
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			m_ColorAttachments.clear();
		}

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		if (!m_Specification.GetColorAttachmentsSpecifications().empty())
		{
			const uint8_t colorAttachmentsSize = m_Specification.GetAttachmentsCountByType(RHIFrameBufferAttachmentType::Color);
			
			m_ColorAttachments.resize(colorAttachmentsSize);

			for (int8_t idx = 0; idx < colorAttachmentsSize; idx++)
			{
				glGenTextures(1, &m_ColorAttachments[idx]);
				glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[idx]);

				glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGB, 
								m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachments[idx], 0);
			}

			if (m_Specification.GetAttachmentsCountByType(RHIFrameBufferAttachmentType::DepthStencil) == 1)
			{

			}
		}
	}
}
