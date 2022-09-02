#include "MainMenuBarWidget.hpp"

#include <Engine/Gui.hpp>

WMainMenuBarWidget::WMainMenuBarWidget()
{
	SetTickEnabled(false);
}

void WMainMenuBarWidget::OnRenderGui(void)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				ExecuteActionDelegate(MainMenuBarAction::Open);
			}

			if (ImGui::MenuItem("Exit")) 
			{

			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
