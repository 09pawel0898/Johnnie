#pragma once

#include "../../../Resources/RHIFrameBuffer.hpp"

namespace Engine::RHI
{
	class OpenGLFrameBuffer : public RHIFrameBuffer
	{
	private:
		uint32_t m_ID = 0;

		std::vector<uint32_t>	m_ColorAttachments;
		uint32_t				m_DepthStencilAttachment;

	public:
		OpenGLFrameBuffer(RHIFrameBufferSpecification&& Specification);
		~OpenGLFrameBuffer();

		virtual void Bind(void) override;
		virtual void Unbind(void) override;

		virtual void Invalidate(void) override;
	};
}