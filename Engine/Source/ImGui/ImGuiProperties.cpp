#include "EnginePCH.hpp"

#include "ImGuiProperties.hpp"
#include "imgui.h"

namespace Engine
{
	void ImGuiProperties::EnableDocking(void)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		bDockingEnabled = true;
	}
}