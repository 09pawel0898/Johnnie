#include "EnginePCH.hpp"

#include "Renderer/Renderer.hpp"
#include "RHITexture.hpp"

#include "../Platform/OpenGL/Resources/OpenGLTexture.hpp"

namespace Engine::RHI
{
	std::unique_ptr<RHITexture2D> RHITexture2D::Create(std::string_view FilePath)
	{
		switch (Renderer::GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLTexture2D>(FilePath); break;
		}
		return nullptr;
	}
}