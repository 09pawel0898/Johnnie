#include "EnginePCH.hpp"

#include "RHIFrameBuffer.hpp"
#include "Renderer/Renderer.hpp"
#include "../Platform/OpenGL/Resources/OpenGLFrameBuffer.hpp"

namespace Engine::RHI
{
	namespace Utility
	{

	}

	std::unique_ptr<RHIFrameBuffer> RHIFrameBuffer::Create(RHIFrameBufferSpecification const& Specification)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLFrameBuffer>(std::move(Specification)); break;
		}
		return nullptr;
	}
	
	uint32_t RHIFrameBufferSpecification::GetAttachmentsCountByType(RHIFrameBufferAttachmentType Type)
	{
		return (uint32_t)std::count_if(AttachmentsSpecification.cbegin(), AttachmentsSpecification.cend(),
			[Type](FramebufferAttachmentSpecification const& Specification)
			{
				return Specification.Type == Type;
			});
	}

	std::vector<FramebufferAttachmentSpecification> RHIFrameBufferSpecification::GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType Type) const
	{
		std::vector<FramebufferAttachmentSpecification> result;

		std::for_each(AttachmentsSpecification.cbegin(), AttachmentsSpecification.cend(),
			[&result, Type](FramebufferAttachmentSpecification const& Specification)
			{
				if (Specification.Type == Type)
				{
					result.emplace_back(Specification);
				}
			});
		return result;
	}
}