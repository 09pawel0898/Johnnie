#include "EnginePCH.hpp"

#include "Renderer.hpp"
#include "Scene/Entities/Camera/CameraController.hpp"

#include "RHI/Resources/RHIShader.hpp"
#include "RHI/RHICommand.hpp"

#include "Core/Debug/ProfileMacros.hpp"

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
		static glm::vec4 defaultClearColor = glm::vec4(0.101f, 0.105f, 0.109f, 1.00f);

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
		Shader->SetFloat3("uCameraPosition", camera->GetLocation());
		Shader->SetMat4("uViewMat", camera->GetViewMat());
		Shader->SetMat4("uProjMat", camera->GetProjectionMat());
		Shader->SetMat4("uModelMat", ModelMat);
		Shader->SetMat3("uNormalMat", glm::mat3(glm::transpose(glm::inverse(ModelMat))));
		
		RHICommand::DrawIndexed(VertexArray);
	}

	RHIShaderManager& Renderer::GetShaderManager(void) const
	{
		return RHICommand::GetRHI()->GetShaderManager();
	}

	RHITexture2DManager& Renderer::GetTexture2DManager(void) const
	{
		return RHICommand::GetRHI()->GetTexture2DManager();
	}

	MaterialManager& Renderer::GetMaterialManager(void) const
	{
		return RHICommand::GetRHI()->GetMaterialManager();
	}

	void Renderer::UpdateRendererStats(void)
	{
		s_RendererStats.FrameDuration			= (double)(GET_PROFILE_RESULT("RendererStats_FrameDuration") / 1000.0);
		s_RendererStats.TickDuration			= (double)(GET_PROFILE_RESULT("RendererStats_TickDuration") / 1000.0);
		s_RendererStats.RenderDuration			= (double)(GET_PROFILE_RESULT("RendererStats_RenderDuration") / 1000.0);
		s_RendererStats.RenderWorldDuration		= (double)(GET_PROFILE_RESULT("RendererStats_RenderWorldDuration") / 1000.0);
		s_RendererStats.RenderGUIDuration		= (double)(GET_PROFILE_RESULT("RendererStats_RenderGUIDuration") / 1000.0);
	}

	RendererStatistics const& Renderer::GetRendererStats(void)
	{
		return s_RendererStats;
	}
}