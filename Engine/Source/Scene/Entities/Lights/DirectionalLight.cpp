#include "EnginePCH.hpp"

#include "DirectionalLight.hpp"
#include "Scene/SceneManager.hpp"

namespace Engine
{

	ADirectionalLight::ADirectionalLight(glm::vec3 const& Direction, glm::vec3 const& LightColor)
		:	Actor(glm::vec3(0.f, 0.f, 0.f)),
			m_LightData(LightColor,Direction)
	{}

	void ADirectionalLight::OnTick(double DeltaTime)
	{}

	void ADirectionalLight::OnConstruct(void)
	{
		RegisterLight();
	}

	void ADirectionalLight::RegisterLight(void)
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();
		CheckMsg(activeScene != nullptr, "No active scene.");

		if (!activeScene->GetLightingManager().IsLightRegistered(GetUUID()))
		{
			activeScene->GetLightingManager().RegisterDirectionalLight(shared_from_this());
		}
	}

	void ADirectionalLight::UnRegisterLight(void)
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();
		CheckMsg(activeScene != nullptr, "No active scene.");

		if (activeScene->GetLightingManager().IsLightRegistered(GetUUID()))
		{
			activeScene->GetLightingManager().UnregisterDirectionalLight(GetUUID());
		}
	}

	void ADirectionalLight::SetVisibility(bool Visible)
	{
		Actor::SetVisibility(Visible);

		if (Visible)
		{
			RegisterLight();
		}
		else
		{
			UnRegisterLight();
		}
	}
	
	glm::mat4 ADirectionalLight::GetDepthVP(void)
	{	
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(-10, 10, 10), m_LightData.Direction, glm::vec3(0, 1, 0));
		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;
		return depthMVP;
	}
}