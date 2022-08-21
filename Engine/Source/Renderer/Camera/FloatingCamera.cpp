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
		const float cameraSpeed = 2.5f * DeltaTime; // adjust accordingly
		
		if (Input::IsKeyPressed(KeyCode::W))
			GetLocation() += cameraSpeed * m_Forward;
		if (Input::IsKeyPressed(KeyCode::S))
			GetLocation() -= cameraSpeed * m_Forward;
		if (Input::IsKeyPressed(KeyCode::A))
			GetLocation() -= glm::normalize(glm::cross(m_Forward, m_Up)) * cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::D))
			GetLocation() += glm::normalize(glm::cross(m_Forward, m_Up)) * cameraSpeed;

		OCamera::OnTick(DeltaTime);
	}

	void OFloatingCamera::OnEvent(Event& Event)
	{
	}
}