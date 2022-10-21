#include "EnginePCH.hpp"

#include "OpenGLTexture.hpp"
#include "Log/Log.hpp"
#include "Renderer/Renderer.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace Engine::RHI
{
	OpenGLTexture2D::OpenGLTexture2D(std::string_view FilePath, RHITextureType TextureType)
		:	RHITexture2D(FilePath,TextureType)
	{
		if (!LoadTextureFromFile())
		{
			LOG(RHI, Error, "Failed to load OpenGLTexture2D from file {0}.", m_FilePath.data());
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::Bind(uint8_t TextureSlotID)
	{
		glActiveTexture(GL_TEXTURE0 + TextureSlotID);
		glBindTexture(GL_TEXTURE_2D, m_ID);	
	}

	void OpenGLTexture2D::Unbind(void)
	{
		UNIMPLEMENTED;
	}

	uint32_t OpenGLTexture2D::GetRendererID(void) const
	{
		return m_ID;
	}

	bool OpenGLTexture2D::operator==(RHITexture const& rhs) const
	{
		OpenGLTexture2D* glTexture = dynamic_cast<OpenGLTexture2D*>(const_cast<RHITexture*>(&rhs));
		if (glTexture)
		{
			return m_ID == glTexture->m_ID;
		}
		return false;
	}

	bool OpenGLTexture2D::LoadTextureFromFile(void)
	{

		int32_t numColChannels;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* bytes = stbi_load(m_FilePath.data(), &m_Width, &m_Height, &numColChannels, 0);

		if (bytes)
		{

			GLenum internalFormat = 0, format = 0;

			if (numColChannels == 4)
			{
				internalFormat = GL_RGBA;
				format = GL_RGBA;
			}
			else if (numColChannels == 3)
			{
				internalFormat = GL_RGB;
				format = GL_RGB;
			}
			else if (numColChannels == 1)
			{ 
				internalFormat = GL_RED;
				format = GL_RED_INTEGER;
			}
				
			glGenTextures(1, &m_ID);
			glBindTexture(GL_TEXTURE_2D, m_ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, internalFormat, GL_UNSIGNED_BYTE, bytes);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(bytes);
			return true;
		}
		return false;
	}

	void OpenGLTexture2D::BindOpenGLTexture(uint32_t RendererID, uint32_t Slot)
	{
		glActiveTexture(GL_TEXTURE0 + Slot);
		glBindTexture(GL_TEXTURE_2D, RendererID);
	}
}