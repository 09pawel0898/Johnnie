#pragma once

#include "RHIResource.hpp"

namespace Engine::RHI
{
	enum class RHIVertexBufferElementType : uint8_t
	{
		None = 0, Int, Int2, Int3, Int4, Bool, Float, Float2, Float3, Float4, Mat3, Mat4,
	};

	size_t RHIGetVertexBufferElementTypeSize(RHIVertexBufferElementType VBOElementType);

	struct RHIVertexBufferElement final
	{
		RHIVertexBufferElementType	VBOElementType;
		std::string_view			ElementName;
		bool						bNormalized;
		size_t						Offset = 0;
		size_t						Size;

		RHIVertexBufferElement() = default;

		RHIVertexBufferElement(RHIVertexBufferElementType Type, std::string_view ElementName, bool Normalized = false)
			:	VBOElementType(Type), ElementName(ElementName), bNormalized(Normalized), Size(RHIGetVertexBufferElementTypeSize(Type))
		{}
	};

	class RHIVertexBufferLayout
	{
	private:
		std::vector<RHIVertexBufferElement> m_Elements;
		size_t m_Stride = 0;

	private:
		void CalculateOffsetAndStride(void);

	public:
		RHIVertexBufferLayout() = default;
		virtual ~RHIVertexBufferLayout() = default;

		RHIVertexBufferLayout(std::initializer_list<RHIVertexBufferElement> Elements);

	public:
		std::vector<RHIVertexBufferElement> const& GetElements(void) const
		{
			return m_Elements;
		}

		size_t GetStride(void) const
		{
			return m_Stride;
		}
	};

	class RHIIndexBuffer : public RHIResource
	{
	private:
		uint32_t m_Count;

	public:
		virtual ~RHIIndexBuffer() = default;

		RHIIndexBuffer(uint32_t Count)
			:	m_Count(Count)
		{}

		[[nodiscard]]
		static  std::unique_ptr<RHIIndexBuffer> Create(uint32_t* Indices, uint32_t Count);

	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		uint32_t GetCount() const
		{
			return m_Count;
		};
	};

	class RHIVertexBuffer : public RHIResource
	{
	public:
		virtual ~RHIVertexBuffer() = default;

		[[nodiscard]]
		static std::unique_ptr<RHIVertexBuffer> Create(uint32_t Size);
		[[nodiscard]]
		static std::unique_ptr<RHIVertexBuffer> Create(const void* Vertices, uint32_t Size);

	public:
		virtual void Bind(void) const = 0;
		virtual void Unbind(void) const = 0;

		virtual void SetData(void const* Data, uint32_t Size) = 0;
		virtual void const* GetData(void) const = 0;

		virtual std::unique_ptr<RHIVertexBufferLayout> const& GetLayout() const = 0;
		virtual void SetLayout(std::unique_ptr<RHIVertexBufferLayout> Layout) = 0;
	};
}