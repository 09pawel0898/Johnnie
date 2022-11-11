#include "EnginePCH.hpp"
#include "EngineBaseLayer.hpp"

#include "Scene/Entities/Camera/CameraController.hpp"
#include "Scene/SceneManager.hpp"
#include "Renderer/Renderer.hpp"

namespace Engine
{
	void EngineBaseLayer::OnAwake(void)
	{
		PrepareShaders();
	}

	void EngineBaseLayer::OnDetach(void)
	{
	}

	void EngineBaseLayer::OnTick(double DeltaTime)
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();

		if (activeScene != nullptr)
		{
			activeScene->Tick(DeltaTime);
		}
		CameraController::Get()->OnTick(DeltaTime);
	}

	void EngineBaseLayer::OnEvent(Events::Event& Event)
	{
		CameraController::Get()->OnEvent(Event);

		auto& activeScene = SceneManager::Get()->GetActiveScene();
		activeScene->OnEvent(Event);
	}

	void EngineBaseLayer::OnRender(void) const
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();

		if (activeScene != nullptr)
		{
			activeScene->Render();
		}
	}

	void EngineBaseLayer::OnRenderGui(void)
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();

		if (activeScene != nullptr)
		{
			activeScene->RenderImGui();
		}
	}

	void EngineBaseLayer::PrepareShaders(void)
	{
		RHI::RHIShaderManager& shaderManager = Renderer::Get()->GetShaderManager();

		shaderManager.LoadResource("Shader_StaticMesh",		"Assets/Shaders/StaticMesh.glsl");
		shaderManager.LoadResource("Shader_EmissiveMesh",	"Assets/Shaders/EmissiveMesh.glsl");
		shaderManager.LoadResource("Shader_Wireframe",		"Assets/Shaders/Wireframe.glsl");
		shaderManager.LoadResource("Shader_ShadowMap",		"Assets/Shaders/ShadowMap.glsl");
		shaderManager.LoadResource("Shader_SkinnedMeshBoneInfluence",		"Assets/Shaders/SkinnedMeshBoneInfluence.glsl");
	}
}