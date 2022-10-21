#pragma once
#include "RHIResource.hpp"

namespace Engine::RHI
{
	enum class RHIFrameBufferAttachmentTextureFormat : uint8_t
	{
		None = 0,

		RGBA8,
		DEPTH24STENCIL8,
		DEPTH16,
	};

	enum class RHIFrameBufferAttachmentType : uint8_t
	{
		Color = 0,
		DepthStencil,
		Depth
	};

	struct FramebufferAttachmentSpecification
	{
		RHIFrameBufferAttachmentType Type;
		RHIFrameBufferAttachmentTextureFormat Format;
	};

	class RHIFrameBufferSpecification
	{
	private:
		std::vector<FramebufferAttachmentSpecification>  AttachmentsSpecification;

	public:
		uint32_t Samples{ 1 };
		uint32_t Width{ 0 };
		uint32_t Height{ 0 };

	public:
		RHIFrameBufferSpecification() = default;

		RHIFrameBufferSpecification(uint32_t Width, uint32_t Height, uint32_t Samples, std::vector<FramebufferAttachmentSpecification> const& AttachmentsSpecification)
			:	Samples(Samples), Width(Width), Height(Height), AttachmentsSpecification(AttachmentsSpecification)
		{}

		uint32_t GetAttachmentsCountByType(RHIFrameBufferAttachmentType Type);

		std::vector<FramebufferAttachmentSpecification> GetAttachmentsSpecificationByType(RHIFrameBufferAttachmentType Type) const;
	};

	class RHIFrameBuffer : public RHIResource
	{
	protected:
		RHIFrameBufferSpecification m_Specification {};

	public:
		RHIFrameBuffer() = delete;

		explicit RHIFrameBuffer(RHIFrameBufferSpecification const& Specification)
			:	m_Specification(Specification)
		{}

		virtual ~RHIFrameBuffer() = default;

		static std::unique_ptr<RHIFrameBuffer> Create(RHIFrameBufferSpecification const& Specification);

	public:
		virtual void Bind(void) = 0;
		virtual void Unbind(void) = 0;

		virtual void Invalidate(void) = 0;

		virtual uint32_t GetRendererID(void) const = 0;

		virtual void ResolveToScreen(void) = 0;
		virtual void ResolveToFramebuffer(std::unique_ptr<RHIFrameBuffer> const& FrameBuffer) = 0;

		virtual uint32_t GetColorAttachmentID(int32_t Index) = 0;
		virtual uint32_t GetDepthStencilAttachmentID(void) = 0;
		virtual uint32_t GetDepthAttachmentID(void) = 0;
	};
}