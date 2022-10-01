#pragma once

#include <Engine/Gui.hpp>

#include "ImGui/ImGuiWidgetBase.hpp"

enum class MainMenuBarAction : uint8_t
{
	Open = 0,
	Exit,
};

class WJohnnieMainMenuBarWidget final : public ImGuiWidgetBase<MainMenuBarAction>
{
private:
	ImGui::FileBrowser m_FileBrowser;
	std::string m_SelectedFileName = "";

	void InitFileBrowser(void);

public:
	WJohnnieMainMenuBarWidget();

	std::string GetSelectedFileName(void) const;

	virtual void OnRenderGui(void) override;
};