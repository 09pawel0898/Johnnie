#pragma once

#include "../../../Resources/RHIBuffers.hpp"

namespace Engine::RHI
{
	class OpenGLVertexBufferLayout final : public RHIVertexBufferLayout
	{};

	class OpenGLIndexBuffer final : public RHIIndexBuffer
	{
	private:
		uint32_t m_ID = 0;

	public:
		OpenGLIndexBuffer(uint32_t* Indices, uint32_t Count);
		OpenGLIndexBuffer(std::vector<uint32_t> const& Indices);
		virtual ~OpenGLIndexBuffer();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
	};

	class OpenGLVertexBuffer final : public RHIVertexBuffer
	{
	private:
		uint32_t m_ID = 0;
		
	public:
		OpenGLVertexBuffer(uint32_t Size);
		OpenGLVertexBuffer(const void* Vertices, uint32_t Size);
		OpenGLVertexBuffer(std::vector<RHIVertex> const& Vertices);

		~OpenGLVertexBuffer();

	public:
		virtual void Bind(void) const override;
		virtual void Unbind(void) const override;

		virtual void SetData(const void* Data, uint32_t Size) override;
		virtual const void* GetData(void) const override;

		virtual TUniquePtr<RHIVertexBufferLayout> const& GetLayout() const override;
		virtual void SetLayout(TUniquePtr<RHIVertexBufferLayout> Layout) override;
	};
}