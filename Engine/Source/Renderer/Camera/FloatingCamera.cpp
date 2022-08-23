#include "EnginePCH.hpp"
#include "FloatingCamera.hpp"

#include "Core/Input/Input.hpp"
#include "Core/Application/Application.hpp"


//temp
#include "GLFW/glfw3.h"

namespace Engine
{
	OFloatingCamera::OFloatingCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation)
		: OCamera(FieldOfView, AspectRatio, NearClip, FarClip, SpawnLocation)
	{
	}

	void OFloatingCamera::OnTick(double DeltaTime)
	{
		const float radius = 3.0f;
		static float mod = 99999;
		static glm::vec2 prevFrameMousePos = glm::vec2(640, 360);

		float camX = sin(mod) * radius;
		float camZ = cos(mod) * radius;

		const float cameraSpeed = 2.5f * DeltaTime; // adjust accordingly


		if (Input::IsKeyPressed(KeyCode::W))
			GetLocation() += cameraSpeed * m_Forward;
		if (Input::IsKeyPressed(KeyCode::S))
			GetLocation() -= cameraSpeed * m_Forward;
		if (Input::IsKeyPressed(KeyCode::A))
			GetLocation() -= glm::normalize(glm::cross(m_Forward, m_Up)) * cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::D))
			GetLocation() += glm::normalize(glm::cross(m_Forward, m_Up)) * cameraSpeed;

		static bool cursorHidden = false;
		if (Input::IsKeyPressed(KeyCode::B))
		{
			if (!cursorHidden)
			{
				glfwSetInputMode((GLFWwindow*)(Core::Application::Get()->GetWindow()->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				Input::SetMousePosition(glm::vec2(640,360));
				cursorHidden = true;
			}
		}
		else
		{
			if (cursorHidden)
			{
				glfwSetInputMode((GLFWwindow*)(Core::Application::Get()->GetWindow()->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				cursorHidden = false;
			}
		}

		glm::vec2 mousePos = Input::GetMousePosition();

		if(cursorHidden)
		{
			float xOffset = mousePos.x - prevFrameMousePos.x;
			float yOffset = prevFrameMousePos.y - mousePos.y;
			prevFrameMousePos = mousePos;

			const float sensitivity = 0.1f;

			xOffset *= sensitivity;
			yOffset *= sensitivity;

			glm::vec3 currentRotation = GetRotation();

			float newPitch = currentRotation.y + yOffset;
			float newYaw = currentRotation.z + xOffset;

			if (newPitch > 89.0f)
				newPitch = 89.0f;
			if (newPitch < -89.0f)
				newPitch = -89.0f;

			SetRotation(glm::vec3(0.f, newPitch, newYaw));
		}
		OCamera::OnTick(DeltaTime);
	}

	void OFloatingCamera::OnEvent(Event& Event)
	{
	}
}