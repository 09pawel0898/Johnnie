#include "Window.hpp"

#include "glad/glad.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../../Events/WindowEvent.hpp"
#include "../../Events/KeyEvent.hpp"
#include "../../Events/MouseEvent.hpp"
#include "../Exceptions/InitializationException.hpp"

#include "CoreGLFW.hpp"

namespace Engine::Core
{
	WindowProperties::WindowProperties(std::string const& Title, unsigned Width, unsigned Height)
		:	Title(Title),
			Width(Width),
			Height(Height),
			bEnableVSync(false),
			EventCallback(nullptr)
	{}

	Window::Window(WindowProperties const& Properties)
	{
		glfwSetErrorCallback(GLFWErrorCallback);

		if (!glfwInit())
		{
			throw InitializationException("Failed to initialize GLFW.");
		}
		
		if (!InitWindowHandle(Properties))
		{
			throw InitializationException("Failed to initialize GLFW window handle.");
		}

		if (!InitOpenGLContext())
		{
			throw InitializationException("Failed to initialize OpenGL context.");
		}
		
		/** GLFW events must be initialized before ImGui context */
		InitEvents();

		if (!InitImGuiContext())
		{
			throw InitializationException("Failed to initialize ImGui context.");
		}
		LOG(Core,Info,"GLFW Window initialized with OpenGL context.")
	}

	Window::~Window()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();
	}

	std::shared_ptr<Window> Window::Create(WindowProperties const& Properties)
	{
		return std::make_shared<Window>(Properties);
	}

	void Window::OnTick(void)
	{
		glfwPollEvents();
		glfwSwapBuffers(m_WindowHandle);
	}

	void Window::OnRenderImGuiFrame()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Window::OnBeginImGuiFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Window::OnEndImGuiFrame()
	{
		ImGui::Render();
	}

	void Window::SetVSync(bool Enabled)
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

	void Window::InitProperties(WindowProperties const& Properties)
	{
		m_Properties = Properties;
		glfwSetWindowUserPointer(m_WindowHandle, &m_Properties);
	}

	void Window::InitEvents()
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
			Events::MouseMovedEvent mouseMovedEvent(NewX, NewY);
			properties->EventCallback(mouseMovedEvent);
		});
	}

	bool Window::InitImGuiContext(void)
	{
		ImGui::CreateContext();
		m_ImGuiIO = &ImGui::GetIO();
		
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_WindowHandle, true);
		ImGui_ImplOpenGL3_Init("#version 150");

		return true;
	}

	bool Window::InitWindowHandle(WindowProperties const& Properties)
	{
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW::CONTEXT_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW::CONTEXT_VERSION_MINOR);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW::WINDOW_RESIZEABLE);

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

	bool Window::InitOpenGLContext()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return false;
		}
		return true;
	}
}