#include "EnginePCH.hpp"
#include "FloatingCamera.hpp"

#include "Core/Input/Input.hpp"
#include "Core/Application/Application.hpp"

//#include "glm/gtx/compatibility.hpp"

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
		static glm::vec2 prevFrameMousePos = glm::vec2(640, 360);

		const float cameraSpeed = m_Speed * DeltaTime; // adjust accordingly
		
		static float cammeraAccelerationMod = 0.1f;

		auto IncreaseAccelerationMod = [this]() 
		{
			cammeraAccelerationMod = cammeraAccelerationMod * (100 + m_Acceleration)/100;
			if (cammeraAccelerationMod > 1.0f)
			{
				cammeraAccelerationMod = 1.0f;
			}
		};
		
		if (Input::IsKeyPressed(KeyCode::W))
		{
			GetLocation() += cameraSpeed * m_Forward * cammeraAccelerationMod;
			IncreaseAccelerationMod();
		}
		else if (Input::IsKeyPressed(KeyCode::S))
		{
			GetLocation() -= cameraSpeed * m_Forward * cammeraAccelerationMod;
			IncreaseAccelerationMod();
		}
		else if (Input::IsKeyPressed(KeyCode::A))
		{
			GetLocation() -= glm::normalize(glm::cross(m_Forward, m_Up)) * cameraSpeed * cammeraAccelerationMod;
			IncreaseAccelerationMod();
		}
		else if (Input::IsKeyPressed(KeyCode::D))
		{
			GetLocation() += glm::normalize(glm::cross(m_Forward, m_Up)) * cameraSpeed * cammeraAccelerationMod;
			IncreaseAccelerationMod();
		}
		else
		{
			cammeraAccelerationMod = std::lerp(cammeraAccelerationMod, 0.1f, (float)(1 - pow(0.8, DeltaTime)));
		}

		static bool test = true;
		if (test)
		{
			glfwSetInputMode((GLFWwindow*)(Core::Application::Get()->GetWindow()->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			Input::SetMousePosition(glm::vec2(640, 360));	
			test = false;
		}

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

		if(cursorHidden || !test)
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