#pragma once

#include "RHIResource.hpp"

#include "Core/CoreMinimal.hpp"

namespace Engine::RHI
{
	class RHIVertexBuffer;
	class RHIIndexBuffer;

	class RHIVertexArray : public RHIResource
	{
	protected:
		std::vector<std::shared_ptr<RHIVertexBuffer>> m_VertexBuffers;
		std::shared_ptr<RHIIndexBuffer> m_IndexBuffer;

	public:
		[[nodiscard]]
		static std::unique_ptr<RHIVertexArray> Create(void);

		virtual ~RHIVertexArray() = default;

	public:
		virtual void Bind(void) const	= 0;
		virtual void Unbind(void) const = 0;

		virtual void SetIndexBuffer(std::shared_ptr<RHIIndexBuffer> IndexBuffer)	= 0;
		virtual void AddVertexBuffer(std::shared_ptr<RHIVertexBuffer> VertexBuffer) = 0;

		std::vector<std::shared_ptr<RHIVertexBuffer>> const& GetVertexBuffers(void) const;

		std::shared_ptr<RHIIndexBuffer> const& GetIndexBuffer(void) const;

		uint32_t GetVertexCount(void);
	};

	FORCEINLINE std::vector<std::shared_ptr<RHIVertexBuffer>> const& RHIVertexArray::GetVertexBuffers(void) const
	{
		return m_VertexBuffers;
	}

	FORCEINLINE std::shared_ptr<RHIIndexBuffer> const& RHIVertexArray::GetIndexBuffer(void) const
	{
		return m_IndexBuffer;
	}
}
