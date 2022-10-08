#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Gui.hpp>

#include "ImGui/ImGuiWidgetBase.hpp"

class WJohnnieSceneWidget final : public ImGuiWidgetBase
{
private:
	/** Point Light */
	std::shared_ptr<APointLight>	m_ManagedPointLight{ nullptr };
	ImColor							m_PointLightColor;
	
	/** Rendering */
	bool m_bWireframeMode{ false };

public:
	WJohnnieSceneWidget();

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;

	void SetManagedPointLight(std::shared_ptr<APointLight> PointLight);

private:
	void SetRendererWireframemode(bool Enabled);
};