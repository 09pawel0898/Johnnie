#pragma once

#include "RHIResource.hpp"
#include "../RHITypes.hpp"

namespace Engine::RHI
{
	class RHITexture : public RHIResource
	{
	protected:
		RHITextureType m_TextureType;
		std::string_view m_FilePath;
		int32_t m_Width;
		int32_t m_Height;

		RHITexture(std::string_view FilePath, RHITextureType TextureType)
			:	m_FilePath(FilePath), m_TextureType(TextureType)
		{}
	
	public:
		virtual ~RHITexture() = default;
		
		RHITexture(RHITexture const&) = delete;
		RHITexture& operator=(RHITexture const&) = delete;

		RHITexture(RHITexture&&) = default;
		RHITexture& operator=(RHITexture&&) = default;

	public:
		std::string_view GetPath(void) const	{	return m_FilePath;		}
		int32_t GetWidth(void) const			{	return m_Width;			}
		int32_t GetHeight(void) const			{	return m_Height;		}
		RHITextureType GetType(void) const		{	return m_TextureType;	}

	public:
		virtual void Bind(uint8_t TextureSlotID) = 0;
		virtual void Unbind(void) = 0;
		virtual uint32_t GetRendererID(void) const = 0;

		virtual bool operator==(RHITexture const& rhs) const = 0;
	};

	class RHITexture2D : public RHITexture
	{
	public:
		RHITexture2D(std::string_view FilePath, RHITextureType TextureType)
			:	RHITexture(FilePath, TextureType)
		{}

	public:
		[[nodiscard]]
		static std::unique_ptr<RHITexture2D> Create(std::string_view FilePath, RHITextureType TextureType = RHITextureType::BasicTexture2D);
	};
}