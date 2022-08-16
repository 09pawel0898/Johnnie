#pragma once

#include "RHIResource.hpp"

namespace Engine::RHI
{
	class RHIVertexBuffer;
	class RHIIndexBuffer;

	class RHIVertexArray : public RHIResource
	{
	protected:
		std::vector<std::unique_ptr<RHIVertexBuffer>> m_VertexBuffers;
		std::unique_ptr<RHIIndexBuffer> m_IndexBuffer;

	public:
		[[nodiscard]]
		static std::unique_ptr<RHIVertexArray> Create(void);

		virtual ~RHIVertexArray() = default;

	public:
		virtual void Bind(void) const = 0;
		virtual void Unbind(void) const = 0;

		virtual void AddVertexBuffer(std::unique_ptr<RHIVertexBuffer> VertexBuffer) = 0;

		virtual void SetIndexBuffer(std::unique_ptr<RHIIndexBuffer> IndexBuffer) = 0;

		virtual std::vector<std::unique_ptr<RHIVertexBuffer>> const& GetVertexBuffers(void) const = 0;
		virtual std::unique_ptr<RHIIndexBuffer> const& GetIndexBuffer(void) const = 0;
	};
}
