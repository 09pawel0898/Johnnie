#include "EnginePCH.hpp"

#include "WindowsWindow.hpp"

#include "Events/WindowEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Core/Exceptions/InitializationException.hpp"
#include "Log/Log.hpp"
#include "Core/Application/ApplicationDelegates.hpp"
#include "../CoreGLFW.hpp"
#include "imgui.h"

namespace Engine::Core
{
	WindowsWindow::WindowsWindow(WindowProperties const& Properties)
	{
		glfwSetErrorCallback(GLFWErrorCallback);

		if (!glfwInit())
		{
			throw InitializationException("Failed to initialize GLFW.");
		}
		
		if (!InitNativeWindow(Properties))
		{
			throw InitializationException("Failed to initialize GLFW window handle.");
		}
		
		/** GLFW events must be initialized before ImGui context */
		InitEvents();

		LOG(Core, Info, "GLFW Window initialized.");
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();
	}

	void WindowsWindow::OnTick(void)
	{
		glfwPollEvents();
		glfwSwapBuffers(m_WindowHandle);
	}

	void WindowsWindow::SetVSync(bool Enabled)
	{
		if (Enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Properties.bEnableVSync = Enabled;
	}

	void WindowsWindow::InitProperties(WindowProperties const& Properties)
	{
		m_Properties = Properties;
		glfwSetWindowUserPointer(m_WindowHandle, &m_Properties);
	}

	void WindowsWindow::InitEvents()
	{
		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* Window)
		{
			auto properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(Window));
			Check(properties);
			Events::WindowClosedEvent windowClosedEvent;
			properties->EventCallback(windowClosedEvent);
		});

		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
		{
			auto properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(Window));
			Check(properties);

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
			Check(properties);

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
			Check(properties);
			Events::MouseMoveTempdEvent mouseMoveTempdEvent(NewX, NewY);
			properties->EventCallback(mouseMoveTempdEvent);
		});

		glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* Window, int Width, int Height)
		{
			auto properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(Window));
			Check(properties);

			properties->Width = Width;
			properties->Height = Height;
			
			Events::WindowResizedEvent windowResizedEvent(Width, Height);
			properties->EventCallback(windowResizedEvent);

			ApplicationDelegates::Get()->OnWindowResized.Broadcast((int32_t)Width, (int32_t)Height);
		});
	}

	bool WindowsWindow::InitNativeWindow(WindowProperties const& Properties)
	{
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW::CONTEXT_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW::CONTEXT_VERSION_MINOR);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE,GLFW_TRUE);

#ifndef NDEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
		glfwWindowHint(GLFW_SAMPLES, 8);

		m_WindowHandle = glfwCreateWindow(	Properties.Width,
											Properties.Height,
											Properties.Title.c_str(),
											nullptr, nullptr);
		if(m_WindowHandle)
		{
			glfwMakeContextCurrent(m_WindowHandle);
			InitProperties(Properties);
			return true;
		}
		return false;
	}
}