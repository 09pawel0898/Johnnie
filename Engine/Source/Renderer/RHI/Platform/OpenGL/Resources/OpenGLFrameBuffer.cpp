#include "EnginePCH.hpp"

#include "OpenGLFrameBuffer.hpp"
#include <glad/glad.h>

namespace Engine::RHI
{
	namespace RHIUtility
	{

	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(RHIFrameBufferSpecification const& Specification)
		:	RHIFrameBuffer(Specification)
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
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		glEnable(GL_DEPTH_TEST);
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
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			m_ColorAttachments.clear();
		}

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		if (!m_Specification.GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType::Color).empty())
		{
			const uint8_t colorAttachmentsSize = m_Specification.GetAttachmentsCountByType(RHIFrameBufferAttachmentType::Color);
			
			m_ColorAttachments.resize(colorAttachmentsSize);

			for (int8_t idx = 0; idx < colorAttachmentsSize; idx++)
			{
				glGenTextures(1, &m_ColorAttachments[idx]);
				glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[idx]);

				if(m_Specification.GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType::Color)[idx].Format == RHIFrameBufferAttachmentTextureFormat::RGBA8)
				{
					glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGB, 
									m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
				}
				else
				{
					Check(false);
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachments[idx], 0);
			}
		}

		if (m_Specification.GetAttachmentsCountByType(RHIFrameBufferAttachmentType::DepthStencil) == 1)
		{
			auto specifications = m_Specification.GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType::DepthStencil);
			Check(specifications.size() == 1);

			FramebufferAttachmentSpecification depthStencilSpecification = specifications[0];
			
			glGenTextures(1, &m_DepthStencilAttachment);
			glBindTexture(GL_TEXTURE_2D, m_DepthStencilAttachment);

			if (depthStencilSpecification.Format == RHIFrameBufferAttachmentTextureFormat::DEPTH24STENCIL8)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
					m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
			}
			else
			{
				Check(false);
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilAttachment, 0);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CheckMsg(false, "Frame buffer not initialized properly.");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
