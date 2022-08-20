#include "EnginePCH.hpp"

#include "RHIShader.hpp"

#include "../../Renderer.hpp"
#include "../Platform/OpenGL/Resources/OpenGLShader.hpp"

namespace Engine::RHI
{
	RHIShader::RHIShader(std::string_view Name)
		:	m_ShaderView(Name)
	{}

	RHIShaderView const& RHIShader::GetShaderView(void)
	{
		return m_ShaderView;
	}

	std::unique_ptr<RHIShader> RHIShader::Create(std::string_view Name, std::string_view FilePath)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLShader>(Name, FilePath); break;
		}
		return nullptr;
	}

	std::unique_ptr<RHIShader> RHIShader::Create(std::string_view Name, std::string_view VertShaderFilePath, std::string_view FragShaderFilePath)
	{
		switch (Renderer::Get()->GetApiType())
		{
			case RenderingAPI::OpenGL: return std::make_unique<OpenGLShader>(Name, VertShaderFilePath, FragShaderFilePath); break;
		}
		return nullptr;
	}
}