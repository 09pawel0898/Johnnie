#pragma once

#include "../../../Resources/RHITexture.hpp"

namespace Engine::RHI
{
	class OpenGLTexture2D final : public RHITexture2D
	{
	private:
		uint32_t m_ID = 0;

	public:
		explicit OpenGLTexture2D(std::string_view FilePath, RHITextureType TextureType);
		~OpenGLTexture2D();

		virtual void Bind(uint8_t TextureSlotID) override;
		virtual void Unbind(void) override;
		virtual uint32_t GetRendererID(void) const override;
		virtual bool operator==(RHITexture const& rhs) const override;
	
	private:
		bool LoadTextureFromFile(void);
	};
}