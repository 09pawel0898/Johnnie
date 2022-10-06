#include "JohnnieMainMenuBarWidget.hpp"

#include <Engine/Utilities.hpp>

WJohnnieMainMenuBarWidget::WJohnnieMainMenuBarWidget()
{
	SetTickEnabled(true);
	InitFileBrowser();
}

std::string WJohnnieMainMenuBarWidget::GetSelectedFileName(void) const
{
	return m_SelectedFileName;
}

void WJohnnieMainMenuBarWidget::OnTick(double DeltaTime)
{
	using namespace Events;

	if (Input::IsKeyPressed(KeyCode::G, KeyCode::LeftCtrl))
	{
		if(!m_bIsFileBrowserOpened)
		{
			m_FileBrowser.Open();
			m_bIsFileBrowserOpened = true;
		}
	}
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
				m_bIsFileBrowserOpened = true;
			}

			if (ImGui::MenuItem("Exit")) 
			{
				// Exit application
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
		ExecuteActionDelegate(MainMenuBarAction::LoadStaticModel);
		m_FileBrowser.ClearSelected();
	}

	if (!m_FileBrowser.IsOpened())
	{
		m_bIsFileBrowserOpened = false;
	}
}

void WJohnnieMainMenuBarWidget::InitFileBrowser(void)
{
	m_FileBrowser.SetTitle("Title");
	m_FileBrowser.SetTypeFilters({ ".obj" });
}