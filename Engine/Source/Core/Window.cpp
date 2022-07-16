#include "Window.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "../Events/WindowEvent.hpp"
#include "../Events/KeyEvent.hpp"
#include "../Events/MouseEvent.hpp"

#include "CoreGLFW.hpp"

namespace Engine::Core
{
	WindowProperties::WindowProperties(const std::string& Title, unsigned Width, unsigned Height)
		:	Title(Title),
			Width(Width),
			Height(Height),
			bEnableVSync(false),
			EventCallback(nullptr)
	{}

	Window::Window(const WindowProperties& Properties)
	{
		CHECK(glfwInit());
		CHECK(InitWindowHandle());
		CHECK(InitOpenGL());

		InitProperties(Properties);

		glfwSetErrorCallback(GLFWErrorCallback);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();
	}

	std::shared_ptr<Window> Window::Create(const WindowProperties& Properties)
	{
		return std::make_shared<Window>(Properties);
	}

	void Window::OnTick(void)
	{
		glfwPollEvents();
		glfwSwapBuffers(m_WindowHandle);
	}

	void Window::SetVSync(bool Enabled)
	{
		if (Enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Properties.bEnableVSync = Enabled;
	}

	void Window::InitProperties(const WindowProperties& Properties)
	{
		m_Properties = Properties;
		glfwSetWindowUserPointer(m_WindowHandle, &m_Properties);
	}

	void Window::InitEvents()
	{
		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* Window)
		{
			auto properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(Window));
			Events::WindowClosedEvent windowClosedEvent;
			properties->EventCallback(windowClosedEvent);
		});

		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
		{
			auto properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(Window));

			switch (Action)
			{
				case GLFW_PRESS:
				{
					Events::KeyPressedEvent keyPressedEvent(static_cast<Events::Keyboard::KeyCode>(Key));
					properties->EventCallback(keyPressedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					Events::KeyReleasedEvent keyReleasedEvent(static_cast<Events::Keyboard::KeyCode>(Key));
					properties->EventCallback(keyReleasedEvent);
					break;
				}
				default: break;
			}
		});

		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* Window, int Button, int Action, int Mods)
		{
			auto properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(Window));

			switch (Action)
			{
			case GLFW_PRESS:
			{
				Events::MouseButtonPressedEvent mouseButtonPressedEvent(static_cast<Events::Mouse::MouseButtonCode>(Button));
				properties->EventCallback(mouseButtonPressedEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				Events::MouseButtonReleasedEvent mouseButtonReleasedEvent(static_cast<Events::Mouse::MouseButtonCode>(Button));
				properties->EventCallback(mouseButtonReleasedEvent);
				break;
			}
			default: break;
			}
		});

		glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* Window, double NewX, double NewY)
		{
			auto properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(Window));
			Events::MouseMovedEvent mouseMovedEvent(NewX, NewY);
			properties->EventCallback(mouseMovedEvent);
		});
	}

	bool Window::InitWindowHandle()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW::CONTEXT_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW::CONTEXT_VERSION_MINOR);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW::WINDOW_RESIZEABLE);

		m_WindowHandle = glfwCreateWindow(	m_Properties.Width,
											m_Properties.Height,
											m_Properties.Title.c_str(),
											nullptr, nullptr);
		if(m_WindowHandle)
		{
			glfwMakeContextCurrent(m_WindowHandle);
			return true;
		}
		return false;
	}

	bool Window::InitOpenGL()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return false;
		}
		return true;
	}
}