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
		std::vector<TSharedPtr<RHIVertexBuffer>> m_VertexBuffers;
		TSharedPtr<RHIIndexBuffer> m_IndexBuffer;

	public:
		[[nodiscard]]
		static TUniquePtr<RHIVertexArray> Create(void);

		virtual ~RHIVertexArray() = default;

	public:
		virtual void Bind(void) const = 0;
		virtual void Unbind(void) const = 0;

		virtual void SetIndexBuffer(TSharedPtr<RHIIndexBuffer> IndexBuffer)	= 0;
		virtual void AddVertexBuffer(TSharedPtr<RHIVertexBuffer> VertexBuffer) = 0;

		std::vector<TSharedPtr<RHIVertexBuffer>> const& GetVertexBuffers(void) const;

		TSharedPtr<RHIIndexBuffer> const& GetIndexBuffer(void) const;

		uint32_t GetVertexCount(void);
	};

	FORCEINLINE std::vector<TSharedPtr<RHIVertexBuffer>> const& RHIVertexArray::GetVertexBuffers(void) const
	{
		return m_VertexBuffers;
	}

	FORCEINLINE TSharedPtr<RHIIndexBuffer> const& RHIVertexArray::GetIndexBuffer(void) const
	{
		return m_IndexBuffer;
	}
}
