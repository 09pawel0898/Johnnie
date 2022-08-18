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
	protected:
		std::unique_ptr<RHIVertexBufferLayout> m_VertexBufferLayout;

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