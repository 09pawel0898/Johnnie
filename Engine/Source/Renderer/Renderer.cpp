#include "EnginePCH.hpp"

#include "Renderer.hpp"
#include "Camera/Camera.hpp"
#include "RHI/Resources/RHIShader.hpp"

#include "RHI/RHICommand.hpp"

namespace Engine
{
	void Renderer::Init(RenderingAPI RenderingAPI)
	{
		Construct();

		RHI::RHICommand::Initialize(RenderingAPI);
	}
	
	void Renderer::Shutdown(void)
	{
		RHI::RHICommand::Shutdown();
	}
	
	std::unique_ptr<DynamicRHI>& Renderer::GetRHI(void)
	{
		return RHI::RHICommand::GetRHI();
	}

	RenderingAPI Renderer::GetApiType(void)
	{
		return RHI::RHICommand::GetRHI()->GetType();
	}

	void Renderer::InitializeViewport(glm::i32vec4 ViewportBounds)
	{
		static glm::vec4 defaultClearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

		Renderer::Get()->GetRHI()->SetClearColor(defaultClearColor);
		Renderer::Get()->GetRHI()->SetViewport(ViewportBounds.x, ViewportBounds.y, ViewportBounds.z, ViewportBounds.w);
	}

	void Renderer::SetViewTarget(std::shared_ptr<OCamera> ViewTarget)
	{
		m_Camera = std::move(ViewTarget);
	}

	void Renderer::Clear(void)
	{
		RHICommand::Clear();
	}

	void Renderer::Draw(std::shared_ptr<RHIShader> const& Shader, std::shared_ptr<RHIVertexArray> const& VertexArray, glm::mat4 const& ModelMat)
	{
		Shader->Bind();
		Shader->SetMat4("model", ModelMat);
		Shader->SetMat4("view", m_Camera->GetViewMat());
		Shader->SetMat4("proj", m_Camera->GetProjectionMat());

		RHICommand::DrawIndexed(VertexArray);
	}
}