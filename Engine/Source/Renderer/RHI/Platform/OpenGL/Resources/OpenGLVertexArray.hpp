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

		virtual void SetIndexBuffer(std::shared_ptr<RHIIndexBuffer> IndexBuffer) override;
		virtual void AddVertexBuffer(std::shared_ptr<RHIVertexBuffer> VertexBuffer) override;

	};
}
