#include "EnginePCH.hpp"

#include "Renderer.hpp"
#include "Scene/Entities/Camera/CameraController.hpp"
#include "Scene/SceneManager.hpp"
#include "RHI/Resources/RHIShader.hpp"
#include "RHI/RHICommand.hpp"
#include "RHI/Resources/RHIFrameBuffer.hpp"

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

		s_Instance = nullptr;
	}
	
	TUniquePtr<DynamicRHI>& Renderer::GetRHI(void)
	{
		return RHICommand::GetRHI();
	}

	RenderingAPI Renderer::GetApiType(void)
	{
		return RHICommand::GetRHI()->GetType();
	}

	void Renderer::InitializeViewport(glm::i32vec4 ViewportBounds)
	{
		RHICommand::SetClearColor(glm::vec4(0.101f, 0.105f, 0.109f, 1.00f));
		RHICommand::SetViewport(ViewportBounds.x, ViewportBounds.y, ViewportBounds.z, ViewportBounds.w);
	}

	void Renderer::InitializeFramebuffer(std::string_view FramebufferName, RHIFrameBufferSpecification const& FramebufferSpecification)
	{
		RHICommand::SetClearColor(glm::vec4(0.101f, 0.105f, 0.109f, 1.00f));
		RHICommand::InitializeFramebuffer(FramebufferName, FramebufferSpecification);
	}

	TUniquePtr<RHIFrameBuffer>& Renderer::GetFramebuffer(std::string_view FramebufferName)
	{
		return RHICommand::GetFramebuffer(FramebufferName);
	}

	void Renderer::BindFramebuffer(std::string_view FramebufferName)
	{
		RHICommand::BindFramebuffer(FramebufferName);
	}

	void Renderer::BindDefaultFramebuffer(void)
	{
		RHICommand::BindDefaultFramebuffer();
	}

	void Renderer::Clear(void)
	{
		RHICommand::Clear();
	}

	void Renderer::Draw(TSharedPtr<RHIShader> const& Shader, TSharedPtr<RHIVertexArray> const& VertexArray, glm::mat4 const& ModelMat)
	{
		if(!HasFlag(R_ShadowMap))
		{
			auto const& Camera = CameraController::Get()->GetCamera();
			Shader->Bind();
			Shader->SetFloat3("uCameraPosition", Camera->GetLocation());
			Shader->SetMat4("uViewMat", Camera->GetViewMat());
			Shader->SetMat4("uProjMat", Camera->GetProjectionMat());
			Shader->SetMat4("uModelMat", ModelMat);
			Shader->SetMat3("uNormalMat", glm::mat3(glm::transpose(glm::inverse(ModelMat))));

			if (SceneManager::Get()->GetActiveScene()->GetLightingManager().GetDirectionalLightDepthVP().has_value())
			{
				static const glm::mat4 BiasMatrix(
					0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 0.5, 0.0,
					0.5, 0.5, 0.5, 1.0
				);

				auto DepthVP = SceneManager::Get()->GetActiveScene()->GetLightingManager().GetDirectionalLightDepthVP().value();
				Shader->SetMat4("uDepthBiasMVP", BiasMatrix * DepthVP * ModelMat);
			}
		}
		else
		{
			Shader->Bind();
			if (SceneManager::Get()->GetActiveScene()->GetLightingManager().GetDirectionalLightDepthVP().has_value())
			{
				auto DepthVP = SceneManager::Get()->GetActiveScene()->GetLightingManager().GetDirectionalLightDepthVP().value();
				Shader->SetMat4("uDepthMVP", DepthVP * ModelMat);
			}
		}
		RHICommand::DrawIndexed(VertexArray);

		s_RendererStats.DrawCalls++;
	}

	void Renderer::OnBeginRenderingFrame(void)
	{
		s_RendererStats.ResetStats();
	}

	void Renderer::OnEndRenderingFrame(void)
	{
		s_RendererStats.UpdateStats();
	}

	void Renderer::OnRenderingFlagsUpdated(RenderingFlags Flags)
	{
		GetRHI()->SetWireframeMode(Flags & R_Wireframe);
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

	RendererStatistics const& Renderer::GetRendererStats(void)
	{
		return s_RendererStats;
	}
	
	RendererStatistics& Renderer::GetMutableRendererStats(void)
	{
		return s_RendererStats;
	}
}