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
}