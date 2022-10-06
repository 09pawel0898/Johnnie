#pragma once

#include <Engine/Gui.hpp>

#include "ImGui/ImGuiWidgetBase.hpp"

enum class MainMenuBarAction : uint8_t
{
	LoadStaticModel = 0,
	Exit,
};

class WJohnnieMainMenuBarWidget final : public ImGuiWidgetBase<MainMenuBarAction>
{
private:
	ImGui::FileBrowser m_FileBrowser;
	bool m_bIsFileBrowserOpened{ false };

	std::string m_SelectedFileName = "";

	void InitFileBrowser(void);

public:
	WJohnnieMainMenuBarWidget();

	std::string GetSelectedFileName(void) const;

	virtual void OnTick(double DeltaTime) override;
	virtual void OnRenderGui(void) override;
};