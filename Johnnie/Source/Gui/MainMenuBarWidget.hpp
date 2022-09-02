#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"

enum class MainMenuBarAction : uint8_t
{
	Open = 0,
	Exit,
};

class WMainMenuBarWidget final : public ImGuiWidgetBase<MainMenuBarAction>
{
public:
	WMainMenuBarWidget();

	virtual void OnRenderGui(void) override;
};