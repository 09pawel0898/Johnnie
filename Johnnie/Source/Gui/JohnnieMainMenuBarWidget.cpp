#include "JohnnieMainMenuBarWidget.hpp"

WJohnnieMainMenuBarWidget::WJohnnieMainMenuBarWidget()
{
	SetTickEnabled(false);
	InitFileBrowser();
}

std::string WJohnnieMainMenuBarWidget::GetSelectedFileName(void) const
{
	return m_SelectedFileName;
}

void WJohnnieMainMenuBarWidget::OnRenderGui(void)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Static Model", "Ctrl + G"))
			{
				m_FileBrowser.Open();
			}

			if (ImGui::MenuItem("Exit")) 
			{

			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	m_FileBrowser.Display();

	if (m_FileBrowser.HasSelected())
	{
		LOG(Core, Trace, "Selected filename {0}",m_FileBrowser.GetSelected().string());
		m_SelectedFileName = m_FileBrowser.GetSelected().string();
		ExecuteActionDelegate(MainMenuBarAction::Open);
		m_FileBrowser.ClearSelected();
	}
}

void WJohnnieMainMenuBarWidget::InitFileBrowser(void)
{
	m_FileBrowser.SetTitle("Title");
	m_FileBrowser.SetTypeFilters({ ".obj" });
}