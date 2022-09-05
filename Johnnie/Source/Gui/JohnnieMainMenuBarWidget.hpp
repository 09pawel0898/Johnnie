#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"

enum class MainMenuBarAction : uint8_t
{
	Open = 0,
	Exit,
};

class WJohnnieMainMenuBarWidget final : public ImGuiWidgetBase<MainMenuBarAction>
{
public:
	WJohnnieMainMenuBarWidget();

	virtual void OnRenderGui(void) override;
};