#include "EnginePCH.hpp"

#include "CameraController.hpp"
#include "Renderer/Camera/FloatingCamera.hpp"

#include "Core/Input/Input.hpp"
#include "Core/Application/Application.hpp"

#include <GLFW/glfw3.h>

namespace Engine
{
	void CameraController::OnEvent(Event& Event)
	{
		Events::EventDispatcher dispatcher(Event);
		
		dispatcher
			.Dispatch<Events::KeyPressedEvent>(BIND_FUNCTION(OnKeyPressed))
			.Dispatch<Events::KeyReleasedEvent>(BIND_FUNCTION(OnKeyReleased));

		if (Event.Handled())
		{
			return;
		}
		else if (m_ControlledCamera)
		{
			m_ControlledCamera->OnEvent(Event);
		}
	}

	bool CameraController::OnKeyPressed(KeyPressedEvent& KeyPressedEvent)
	{
		if (m_ControlledCamera && KeyPressedEvent.GetKeyCode() == KeyCode::B)
		{
			if (!m_bCursorCaputred)
			{
				glfwSetInputMode((GLFWwindow*)(Core::Application::Get()->GetWindow()->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				Input::SetMousePosition(glm::vec2(640, 360));
				m_ControlledCamera->SetCameraPossessed(true);
				m_bCursorCaputred = true;
				m_bSkipNextTickFrame = true;
				return true;
			}
		}
		return false;
	}

	bool CameraController::OnKeyReleased(KeyReleasedEvent& KeyReleasedEvent)
	{
		if (m_ControlledCamera && KeyReleasedEvent.GetKeyCode() == KeyCode::B)
		{
			if (m_bCursorCaputred)
			{
				glfwSetInputMode((GLFWwindow*)(Core::Application::Get()->GetWindow()->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				m_bCursorCaputred = false;
				m_ControlledCamera->SetCameraPossessed(false);
				return true;
			}
		}
		return false;
	}

	void CameraController::OnTick(double DeltaTime)
	{
		if (m_ControlledCamera)
		{
			glm::vec2 mousePos = Input::GetMousePosition();

			if (m_ControlledCamera->IsCameraPossessed())
			{
				/** Fix glfw cursor teleport causing applying big X and Y offset values to rotation in first frame */
				if (m_bSkipNextTickFrame)
				{
					m_bSkipNextTickFrame = false;
					m_PrevFrameMousePos = mousePos;
					return;
				}

				/** Calc X and Y offset rate and apply input to camera */
				float xOffset = mousePos.x - m_PrevFrameMousePos.x;
				float yOffset = m_PrevFrameMousePos.y - mousePos.y;
				m_PrevFrameMousePos = mousePos;
				
				if (auto floatingCamera = Cast<OFloatingCamera>(m_ControlledCamera))
				{
					floatingCamera->OnMouseYawInput(xOffset);
					floatingCamera->OnMousePitchInput(yOffset);
				}
			}
		}
	}
}
