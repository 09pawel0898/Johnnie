#include "ImGuiLayer.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include "../Core/Window/Application.hpp"
#include "../Events/EventsIdentifiers.hpp"

namespace Engine 
{
	using namespace Core;

	void ImGuiLayer::OnAwake()
	{
		/** Initialization of ImGui Context */

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  
		
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		auto application = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(application->GetWindow()->GetWindowHandle());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Events::Event& Event)
	{
		ImGuiIO& io = ImGui::GetIO();
		Event.m_Handled |= io.WantCaptureKeyboard & Event.HasCategory(Events::EventCategory_Keyboard);
		Event.m_Handled |= io.WantCaptureMouse & Event.HasCategory(Events::EventCategory_Mouse);
	}

	void ImGuiLayer::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::EndFrame()
	{
		auto application = Application::Get();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)application->GetWindow()->GetWidth(), 
								(float)application->GetWindow()->GetHeight());

		/** Rendering ImGui Frame */
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}