#pragma once

#include "RHIResource.hpp"
#include "../RHITypes.hpp"

namespace Engine::RHI
{
	size_t RHIGetVertexBufferElementTypeSize(RHIElementType VBOElementType);

	struct RHIVertexBufferElement final
	{
		RHIElementType				VBOElementType;
		std::string_view			ElementName;
		bool						bNormalized;
		size_t						Offset = 0;
		size_t						Size;

		RHIVertexBufferElement() = default;

		RHIVertexBufferElement(RHIElementType Type, std::string_view ElementName, bool Normalized = false)
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
		RHIVertexBufferLayout() = delete;
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

		using Iterator		= std::vector<RHIVertexBufferElement>::iterator;
		using ConstIterator = std::vector<RHIVertexBufferElement>::const_iterator;

		Iterator		begin()			{ return m_Elements.begin();}
		Iterator		end()			{ return m_Elements.end();	}
		ConstIterator	begin() const	{ return m_Elements.begin();}
		ConstIterator	end()	const	{ return m_Elements.end();	}
	};

	class RHIIndexBuffer : public RHIResource
	{
	protected:
		uint32_t m_Count;

	public:
		virtual ~RHIIndexBuffer() = default;

		RHIIndexBuffer(uint32_t Count)
			:	m_Count(Count)
		{}

		[[nodiscard]]
		static  TUniquePtr<RHIIndexBuffer> Create(uint32_t* Indices, uint32_t Count);
		[[nodiscard]]
		static  TUniquePtr<RHIIndexBuffer> Create(std::vector<uint32_t> const& Indices);

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
	protected:
		TUniquePtr<RHIVertexBufferLayout> m_VertexBufferLayout;
		uint32_t m_VerticesCount = 0;

	public:
		virtual ~RHIVertexBuffer() = default;

		[[nodiscard]]
		static TUniquePtr<RHIVertexBuffer> Create(uint32_t Size);
		[[nodiscard]]
		static TUniquePtr<RHIVertexBuffer> Create(const void* Vertices, uint32_t Size);
		[[nodiscard]]
		static TUniquePtr<RHIVertexBuffer> Create(std::vector<RHIVertex> const& Vertices);

	public:
		virtual void Bind(void) const = 0;
		virtual void Unbind(void) const = 0;

		virtual void SetData(const void* Data, uint32_t Size) = 0;
		virtual void const* GetData(void) const = 0;

		virtual TUniquePtr<RHIVertexBufferLayout> const& GetLayout() const = 0;
		virtual void SetLayout(TUniquePtr<RHIVertexBufferLayout> Layout) = 0;

		uint32_t GetVerticesCount(void) const;
	};

	FORCEINLINE uint32_t RHIVertexBuffer::GetVerticesCount(void) const
	{
		return m_VerticesCount;
	}
}