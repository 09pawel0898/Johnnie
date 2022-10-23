#pragma once

#include "Renderer/RHI/Resources/RHIVertexArray.hpp"

namespace Engine::RHI
{
	class OpenGLVertexArray final : public RHIVertexArray
	{
	private:
		uint32_t m_ID;
		uint32_t m_VertexBufferIndex = 0;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetIndexBuffer(TSharedPtr<RHIIndexBuffer> IndexBuffer) override;
		
		virtual void AddVertexBuffer(TSharedPtr<RHIVertexBuffer> VertexBuffer) override;
	};
}
