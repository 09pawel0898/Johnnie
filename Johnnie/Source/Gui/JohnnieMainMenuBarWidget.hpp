#pragma once

#include <Engine/Gui.hpp>
#include <Engine/Utilities.hpp>

#include "ImGui/ImGuiWidgetBase.hpp"

enum class FileBrowserSelectedAssetType : uint8_t
{
	StaticMesh,
	SkeletalMesh
};

class WJohnnieMainMenuBarWidget final : public ImGuiWidgetBase
{
private:
	std::string			m_SelectedFileName = "";
	ImGui::FileBrowser	m_FileBrowser;
	bool				m_bIsFileBrowserOpened{ false };

	FileBrowserSelectedAssetType m_FileBrowserSelectedAssetType;

public:
	WJohnnieMainMenuBarWidget();

	virtual void OnTick(double DeltaTime) override;
	virtual void OnRenderGui(void) override;

private:
	void OpenFileBrowserForAssetType(FileBrowserSelectedAssetType AssetType);
	void NotifyFileBrowserAssetSelected();
};