#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"

enum class MainMenuBarAction : uint8_t
{
	Open = 0,
	Exit,
};

class MainMenuBarWidget final : public ImGuiWidgetBase<MainMenuBarAction>
{
public:
	MainMenuBarWidget();

	virtual void OnRenderGui(void) override;
};