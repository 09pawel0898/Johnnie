#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"

class WJohnnieViewportWidget final : public ImGuiWidgetBase
{
private:

public:
	WJohnnieViewportWidget();

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;
};