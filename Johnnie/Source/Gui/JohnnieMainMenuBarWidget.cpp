#include "JohnnieMainMenuBarWidget.hpp"

#include <Engine/Gui.hpp>

WJohnnieMainMenuBarWidget::WJohnnieMainMenuBarWidget()
{
	SetTickEnabled(false);
}

void WJohnnieMainMenuBarWidget::OnRenderGui(void)
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
