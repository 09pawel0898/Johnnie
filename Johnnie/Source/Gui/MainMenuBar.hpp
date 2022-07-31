#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"

enum class MainMenuBarActions : uint8_t
{
	Open = 0,
	Exit,
};

class MainMenuBar : public ImGuiWidgetBase<MainMenuBarActions>
{
public:
	virtual void OnRenderGui(void) override;
};