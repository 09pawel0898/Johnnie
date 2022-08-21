#include "EnginePCH.hpp"

#include "Renderer.hpp"
#include "Camera/Camera.hpp"
#include "Camera/CameraController.hpp"

#include "RHI/Resources/RHIShader.hpp"
#include "RHI/RHICommand.hpp"

namespace Engine
{
	void Renderer::Init(RenderingAPI RenderingAPI)
	{
		Construct();

		RHICommand::Initialize(RenderingAPI);
	}
	
	void Renderer::Shutdown(void)
	{
		RHICommand::Shutdown();
	}
	
	std::unique_ptr<DynamicRHI>& Renderer::GetRHI(void)
	{
		return RHICommand::GetRHI();
	}

	RenderingAPI Renderer::GetApiType(void)
	{
		return RHICommand::GetRHI()->GetType();
	}

	void Renderer::InitializeViewport(glm::i32vec4 ViewportBounds)
	{
		static glm::vec4 defaultClearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

		RHICommand::SetClearColor(defaultClearColor);
		RHICommand::SetViewport(ViewportBounds.x, ViewportBounds.y, ViewportBounds.z, ViewportBounds.w);
	}

	void Renderer::Clear(void)
	{
		RHICommand::Clear();
	}

	void Renderer::Draw(std::shared_ptr<RHIShader> const& Shader, std::shared_ptr<RHIVertexArray> const& VertexArray, glm::mat4 const& ModelMat)
	{
		auto const& camera = CameraController::Get()->GetCamera();

		Shader->Bind();
		Shader->SetMat4("model", ModelMat);
		Shader->SetMat4("view", camera->GetViewMat());
		Shader->SetMat4("proj", camera->GetProjectionMat());

		RHICommand::DrawIndexed(VertexArray);
	}
}