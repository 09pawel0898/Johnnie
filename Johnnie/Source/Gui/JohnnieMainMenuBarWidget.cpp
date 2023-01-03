#include "JohnnieMainMenuBarWidget.hpp"
#include "../JohnnieDelegates.hpp"
#include "Core/Application/Application.hpp"

#include <Engine/Utilities.hpp>

WJohnnieMainMenuBarWidget::WJohnnieMainMenuBarWidget()
{
	SetTickEnabled(true);
}

void WJohnnieMainMenuBarWidget::OnTick(double DeltaTime)
{
	using namespace Events;

	if (Input::IsKeyPressed(KeyCode::G, KeyCode::LeftCtrl))
	{
		if(!m_bIsFileBrowserOpened)
		{
			OpenFileBrowserForAssetType(FileBrowserSelectedAssetType::StaticMesh);
		}
	}
	if (Input::IsKeyPressed(KeyCode::H, KeyCode::LeftCtrl))
	{
		if(!m_bIsFileBrowserOpened)
		{
			OpenFileBrowserForAssetType(FileBrowserSelectedAssetType::SkeletalMesh);
		}
	}
}

void WJohnnieMainMenuBarWidget::OnRenderGui(void)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Static Mesh", "Ctrl + G"))
			{
				OpenFileBrowserForAssetType(FileBrowserSelectedAssetType::StaticMesh);
			}
			if (ImGui::MenuItem("Load Skeletal Mesh", "Ctrl + H"))
			{
				OpenFileBrowserForAssetType(FileBrowserSelectedAssetType::SkeletalMesh);
			}
			if (ImGui::MenuItem("Exit")) 
			{
				Core::Application::Get()->ShutdownApplication();
			}
			ImGui::EndMenu();
		}
		//if (ImGui::BeginMenu("Style"))
		//{
		//	ImGuiProperties& GUIProperties = ImGuiRenderer::Get()->GetImGuiProperties();
		//	bool DarkMode = GUIProperties.IsDarkMode();
		//
		//	if (ImGui::Checkbox("asd", &DarkMode))
		//	{
		//		GUIProperties.SetDarkMode(DarkMode);
		//	}
		//	ImGui::EndMenu();
		//}
		ImGui::EndMainMenuBar();
	}

	m_FileBrowser.Display();

	if (m_FileBrowser.HasSelected())
	{
		NotifyFileBrowserAssetSelected();
	}

	if (!m_FileBrowser.IsOpened())
	{
		m_bIsFileBrowserOpened = false;
	}
}

void WJohnnieMainMenuBarWidget::OpenFileBrowserForAssetType(FileBrowserSelectedAssetType AssetType)
{
	if (AssetType == FileBrowserSelectedAssetType::StaticMesh)
	{
		m_FileBrowser.SetTypeFilters({ ".obj",".fbx",".dae" });
		m_FileBrowser.SetTitle("Select static model file");
	}
	if (AssetType == FileBrowserSelectedAssetType::SkeletalMesh)
	{
		m_FileBrowser.SetTypeFilters({ ".dae" });
		m_FileBrowser.SetTitle("Select skeletal model file");
	}
	
	m_FileBrowserSelectedAssetType = AssetType;

	m_FileBrowser.Open();
	m_bIsFileBrowserOpened = true;
}

void WJohnnieMainMenuBarWidget::NotifyFileBrowserAssetSelected()
{
	m_SelectedFileName = m_FileBrowser.GetSelected().string();

	if (m_FileBrowserSelectedAssetType == FileBrowserSelectedAssetType::StaticMesh)
	{
		LOG(Core, Trace, "Loading static model from {0}", m_FileBrowser.GetSelected().string());

		JohnnieDelegates::Get()->OnStaticMeshToLoadPathSelected.Broadcast(m_SelectedFileName);
	}
	if (m_FileBrowserSelectedAssetType == FileBrowserSelectedAssetType::SkeletalMesh)
	{
		LOG(Core, Trace, "Loading skeletal model from {0}", m_FileBrowser.GetSelected().string());

		JohnnieDelegates::Get()->OnSkeletalMeshToLoadPathSelected.Broadcast(m_SelectedFileName);
	}

	m_FileBrowser.ClearSelected();
}