#pragma once
#include "RHIResource.hpp"

namespace Engine::RHI
{
	enum class RHIFrameBufferAttachmentTextureFormat : uint8_t
	{
		None = 0,

		RGBA8,
		DEPTH24STENCIL8
	};

	enum class RHIFrameBufferAttachmentType : uint8_t
	{
		Color = 0,
		DepthStencil
	};

	struct FramebufferAttachmentSpecification
	{
		RHIFrameBufferAttachmentType Type;
		RHIFrameBufferAttachmentTextureFormat Format;
	};

	class RHIFrameBufferSpecification
	{
	private:
		std::vector<FramebufferAttachmentSpecification>  Attachments;

	public:
		uint32_t Samples{ 1 };
		uint32_t Width{ 0 };
		uint32_t Height{ 0 };

	public:
		RHIFrameBufferSpecification() = default;

		RHIFrameBufferSpecification(uint32_t Width, uint32_t Height, uint32_t Samples, std::vector<FramebufferAttachmentSpecification> const& Specification)
			:	Samples(Samples), Width(Width), Height(Height), Attachments(Attachments)
		{}

		uint32_t GetAttachmentsCountByType(RHIFrameBufferAttachmentType Type);

		std::vector<FramebufferAttachmentSpecification> GetColorAttachmentsSpecifications(void) const;
		FramebufferAttachmentSpecification GetDepthStencilAttachmentsSpecification(void) const;
	};

	class RHIFrameBuffer : public RHIResource
	{
	protected:
		RHIFrameBufferSpecification m_Specification {};

	public:
		RHIFrameBuffer() = delete;

		explicit RHIFrameBuffer(RHIFrameBufferSpecification&& Specification)
			:	m_Specification(std::move(Specification))
		{}

		virtual ~RHIFrameBuffer() = default;

		static std::unique_ptr<RHIFrameBuffer> Create(RHIFrameBufferSpecification&& Specification);

	public:
		virtual void Bind(void) = 0;
		virtual void Unbind(void) = 0;

		virtual void Invalidate(void) = 0;
	};
}