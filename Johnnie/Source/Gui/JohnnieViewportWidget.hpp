#pragma once

#include <Engine/Gui.hpp>
#include "ImGui/ImGuiWidgetBase.hpp"

class WJohnnieViewportWidget final : public ImGuiWidgetBase
{
private:
	ImVec2 m_ViewportSize;
	ImVec2 m_PrevFrameViewportSize;

public:
	WJohnnieViewportWidget();

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;

public:
	glm::vec2 GetViewportSize(void) const;
};

FORCEINLINE glm::vec2 WJohnnieViewportWidget::GetViewportSize(void) const
{
	return glm::vec2(m_ViewportSize.x, m_ViewportSize.y);
}