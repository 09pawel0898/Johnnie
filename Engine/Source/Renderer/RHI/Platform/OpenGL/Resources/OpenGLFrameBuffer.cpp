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
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		
		if (m_Specification.FaceCullingType == RHIFaceCullingType::None)
		{
			glDisable(GL_CULL_FACE);
		}
		else if (m_Specification.FaceCullingType == RHIFaceCullingType::Front)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}
		else if (m_Specification.FaceCullingType == RHIFaceCullingType::Back)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		GLuint flags = 0x0;
		if (m_ColorAttachments.size() != 0)
		{
			flags |= GL_COLOR_BUFFER_BIT;
		}
		if (m_DepthStencilAttachment)
		{
			flags |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		}
		if (m_DepthAttachment)
		{
			flags |= GL_DEPTH_BUFFER_BIT;
		}

		glClear(flags); // we're not using the stencil buffer now
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
			if (m_DepthStencilAttachment != 0)
			{
				glDeleteTextures(1, &m_DepthStencilAttachment);
			}
			m_ColorAttachments.clear();
		}

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		bool isMultiSample = m_Specification.Samples > 1;

		if (!m_Specification.GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType::Color).empty())
		{
			const uint8_t colorAttachmentsSize = m_Specification.GetAttachmentsCountByType(RHIFrameBufferAttachmentType::Color);
			
			m_ColorAttachments.resize(colorAttachmentsSize);

			for (int8_t idx = 0; idx < colorAttachmentsSize; idx++)
			{
				glGenTextures(1, &m_ColorAttachments[idx]);
				glBindTexture(isMultiSample? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_ColorAttachments[idx]);

				if(m_Specification.GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType::Color)[idx].Format == RHIFrameBufferAttachmentTextureFormat::RGBA8)
				{
					if (!isMultiSample)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
							m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
					
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					}
					else
					{
						glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.Samples, GL_RGBA8, m_Specification.Width, m_Specification.Height, GL_FALSE);
					}
				}
				else
				{
					Check(false);
				}
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, isMultiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_ColorAttachments[idx], 0);
			}
		}

		if (m_Specification.GetAttachmentsCountByType(RHIFrameBufferAttachmentType::DepthStencil) == 1)
		{
			auto specifications = m_Specification.GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType::DepthStencil);
			Check(specifications.size() == 1);

			FramebufferAttachmentSpecification depthStencilSpecification = specifications[0];
			
			glGenTextures(1, &m_DepthStencilAttachment);
			glBindTexture(isMultiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_DepthStencilAttachment);

			if (depthStencilSpecification.Format == RHIFrameBufferAttachmentTextureFormat::DEPTH24STENCIL8)
			{
				if (!isMultiSample)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
						m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
					
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				}
				else
				{
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.Samples, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, GL_FALSE);
				}
			}
			else
			{
				Check(false);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, isMultiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_DepthStencilAttachment, 0);
		}
		
		if (m_Specification.GetAttachmentsCountByType(RHIFrameBufferAttachmentType::Depth) == 1)
		{
			auto specifications = m_Specification.GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType::Depth);
			Check(specifications.size() == 1);

			FramebufferAttachmentSpecification depthSpecification = specifications[0];
			
			glGenTextures(1, &m_DepthAttachment);
			glBindTexture(isMultiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_DepthAttachment);

			if (depthSpecification.Format == RHIFrameBufferAttachmentTextureFormat::DEPTH16)
			{
				if (!isMultiSample)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
						m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
					
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				}
				else
				{
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.Samples, GL_DEPTH_COMPONENT16, m_Specification.Width, m_Specification.Height, GL_FALSE);
				}
			}
			else
			{
				Check(false);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, isMultiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_DepthAttachment, 0);
		}

		if (m_ColorAttachments.size() == 0)
		{
			glReadBuffer(GL_NONE);
			glDrawBuffer(GL_NONE);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CheckMsg(false, "Frame buffer not initialized properly.");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t OpenGLFrameBuffer::GetRendererID(void) const
	{
		return m_ID;
	}

	void OpenGLFrameBuffer::ResolveToScreen(void)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void OpenGLFrameBuffer::ResolveToFramebuffer(std::unique_ptr<RHIFrameBuffer> const& FrameBuffer)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBuffer->GetRendererID());

		glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	uint32_t OpenGLFrameBuffer::GetColorAttachmentID(int32_t Index)
	{
		CheckMsg(m_ColorAttachments.size() > Index, "Color attachment of requested index is not available.");
		
		return m_ColorAttachments[Index];
	}

	uint32_t OpenGLFrameBuffer::GetDepthStencilAttachmentID(void)
	{
		CheckMsg(m_DepthStencilAttachment != 0, "DepthStencil attachment is not available.");

		return m_DepthStencilAttachment;
	}

	uint32_t OpenGLFrameBuffer::GetDepthAttachmentID(void)
	{
		CheckMsg(m_DepthAttachment != 0, "DepthStencil attachment is not available.");

		return m_DepthAttachment;
	}
}
