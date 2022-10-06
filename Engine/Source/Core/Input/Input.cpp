#include "EnginePCH.hpp"

#include "Input.hpp"
#include "../Application/Application.hpp"

#include "GLFW/glfw3.h"

#ifdef PLATFORM_WINDOWS
#	include	"Windows.h"
#endif

namespace Engine
{
	glm::vec2 Input::GetMousePosition(void)
	{
		GLFWwindow* windowHandle = static_cast<GLFWwindow*>(Core::Application::Get()->GetWindow()->GetNativeWindow());
		Check(windowHandle);

		double xPos, yPos;
		glfwGetCursorPos(	windowHandle,
							&xPos,
							&yPos);

		return { (float)xPos,(float)yPos };
	}

	void Input::SetMousePosition(glm::vec2 MousePos)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Core::Application::Get()->GetWindow()->GetNativeWindow());
		glfwSetCursorPos(window, MousePos.x, MousePos.y);
	}


	bool Input::IsKeyPressed(Events::KeyCode KeyCode, Events::KeyCode OptionalSpecialKey)
	{
		GLFWwindow* windowHandle = static_cast<GLFWwindow*>(Core::Application::Get()->GetWindow()->GetNativeWindow());
		Check(windowHandle);

		int mainKeyState = glfwGetKey(windowHandle, (int16_t)KeyCode);

		if (OptionalSpecialKey == Events::KeyCode::None)
		{
			return mainKeyState == GLFW_PRESS || mainKeyState == GLFW_REPEAT;
		}
		else
		{
			int specialKeyState = glfwGetKey(windowHandle, (int16_t)OptionalSpecialKey);
			return ((mainKeyState == GLFW_PRESS || mainKeyState == GLFW_REPEAT)
				&&(specialKeyState == GLFW_PRESS || specialKeyState == GLFW_REPEAT));
		}
	}

	bool Input::IsMouseButtonPressed(Events::MouseButtonCode MouseButtonCode)
	{
		GLFWwindow* windowHandle = static_cast<GLFWwindow*>(Core::Application::Get()->GetWindow()->GetNativeWindow());
		Check(windowHandle);

		int keyState = glfwGetMouseButton(windowHandle, (int16_t)MouseButtonCode);
		return keyState == GLFW_PRESS;
	}
}