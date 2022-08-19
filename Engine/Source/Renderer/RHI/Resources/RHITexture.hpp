#pragma once

#include "RHIResource.hpp"

namespace Engine::RHI
{
	class RHITexture : public RHIResource
	{
	protected:
		std::string_view m_FilePath;
		int32_t m_Width;
		int32_t m_Height;

		RHITexture() = default;
	
	public:
		virtual ~RHITexture() = default;
		
		RHITexture(RHITexture const&) = delete;
		RHITexture& operator=(RHITexture const&) = delete;

		RHITexture(RHITexture&&) = default;
		RHITexture& operator=(RHITexture&&) = default;

	public:
		std::string_view GetPath(void) const	{return m_FilePath;		}
		int32_t GetWidth(void) const			{return m_Width;	}
		int32_t GetHeight(void) const			{return m_Height;	}

	public:
		virtual void Bind(uint8_t TextureSlotID) = 0;
		virtual bool operator==(RHITexture const& rhs) const = 0;
	};

	class RHITexture2D : public RHITexture
	{
	public:
		[[nodiscard]]
		static std::unique_ptr<RHITexture2D> Create(std::string_view FilePath);
	};
}