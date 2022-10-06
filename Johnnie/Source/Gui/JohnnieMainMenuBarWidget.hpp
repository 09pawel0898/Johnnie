#pragma once

#include <Engine/Gui.hpp>
#include <Engine/Utilities.hpp>

#include "ImGui/ImGuiWidgetBase.hpp"

DECLARE_DELEGATE(OnStaticModelToLoadSelected, std::string const&);

class WJohnnieMainMenuBarWidget final : public ImGuiWidgetBase
{
private:
	ImGui::FileBrowser m_FileBrowser;
	bool m_bIsFileBrowserOpened{ false };

	std::string m_SelectedFileName = "";

	void InitFileBrowser(void);

public:
	WJohnnieMainMenuBarWidget();

	virtual void OnTick(double DeltaTime) override;
	virtual void OnRenderGui(void) override;

public:
	OnStaticModelToLoadSelected OnStaticModelToLoadSelected;
};