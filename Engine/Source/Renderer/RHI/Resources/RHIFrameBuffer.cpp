#include "EnginePCH.hpp"

#include "RHIFrameBuffer.hpp"
#include "Renderer/Renderer.hpp"
#include "../Platform/OpenGL/Resources/OpenGLFrameBuffer.hpp"

namespace Engine::RHI
{
	std::unique_ptr<RHIFrameBuffer> RHIFrameBuffer::Create(RHIFrameBufferSpecification&& Specification)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLFrameBuffer>(std::move(Specification)); break;
		}
		return nullptr;
	}
	
	uint32_t RHIFrameBufferSpecification::GetAttachmentsCountByType(RHIFrameBufferAttachmentType Type)
	{
		return std::count_if(Attachments.cbegin(), Attachments.cend(), 
			[Type](FramebufferAttachmentSpecification const& Specification)
			{
				return Specification.Type == Type;
			});
	}
	std::vector<FramebufferAttachmentSpecification> RHIFrameBufferSpecification::GetColorAttachmentsSpecifications(void) const
	{
		return std::vector<FramebufferAttachmentSpecification>();
	}
	FramebufferAttachmentSpecification RHIFrameBufferSpecification::GetDepthStencilAttachmentsSpecification(void) const
	{
		return FramebufferAttachmentSpecification();
	}
}