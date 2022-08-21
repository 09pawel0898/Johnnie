#include "EnginePCH.hpp"
#include "FloatingCamera.hpp"

#include "Core/Input/Input.hpp"


//temp
#include "GLFW/glfw3.h"

namespace Engine
{
	OFloatingCamera::OFloatingCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation)
		:	OCamera(FieldOfView, AspectRatio, NearClip, FarClip, SpawnLocation)
	{
	}

	void OFloatingCamera::OnTick(double DeltaTime)
	{
		const float radius = 3.0f;
		static float mod = 99999;

		float camX = sin(mod) * radius;
		float camZ = cos(mod) * radius;

		//SetLocation(glm::vec3(camX, 0.0f, camZ));

		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		const float cameraSpeed = 2.5f * DeltaTime; // adjust accordingly
		
		if (Input::IsKeyPressed(KeyCode::S))
			GetLocation() += cameraSpeed * cameraFront;
		if (Input::IsKeyPressed(KeyCode::W))
			GetLocation() -= cameraSpeed * cameraFront;
		if (Input::IsKeyPressed(KeyCode::D))
			GetLocation() -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::A))
			GetLocation() += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

		//	auto const& currentLocation = GetLocation();
		//	
		//	m_Target.x + 0.02f;
		//	SetLocation(glm::vec3(currentLocation.x + 0.02f , currentLocation.y, currentLocation.z));
		//}

		OCamera::OnTick(DeltaTime);
	}

	void OFloatingCamera::OnEvent(Event& Event)
	{
	}
}