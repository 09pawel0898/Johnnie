#include "MainMenuBar.hpp"

#include "Engine/Gui.hpp"
#include <iostream>

void MainMenuBar::OnRenderGui(void)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				CallAction(MainMenuBarActions::Open);
			}

			if (ImGui::MenuItem("Exit")) 
			{

			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
