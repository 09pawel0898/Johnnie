#include "EnginePCH.hpp"

#include "Renderer/Renderer.hpp"
#include "RHITexture.hpp"

#include "../Platform/OpenGL/Resources/OpenGLTexture.hpp"

namespace Engine::RHI
{
	TUniquePtr<RHITexture2D> RHITexture2D::Create(std::string_view FilePath, RHITextureType TextureType)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return MakeUnique<OpenGLTexture2D>(FilePath, TextureType); break;
		}
		return nullptr;
	}
}