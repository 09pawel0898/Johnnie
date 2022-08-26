#include "EnginePCH.hpp"

#include "OpenGLTexture.hpp"
#include "Log/Log.hpp"

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
			GLenum dataFormat = 0;

			if (numColChannels == 4)
			{
				dataFormat = GL_RGBA;
			}
			else if (numColChannels == 3)
			{
				dataFormat = GL_RGB;
			}

			glGenTextures(1, &m_ID);
			glBindTexture(GL_TEXTURE_2D, m_ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, bytes);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(bytes);
			return true;
		}
		return false;
	}
}