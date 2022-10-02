#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Gui.hpp>

#include "ImGui/ImGuiWidgetBase.hpp"

enum class SceneAction : uint8_t
{};

class WJohnnieSceneWidget final : public ImGuiWidgetBase<SceneAction>
{
private:
	std::shared_ptr<APointLight>	m_ManagedPointLight{ nullptr };
	ImColor							m_PointLightColor;

public:
	WJohnnieSceneWidget();

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;

	void SetManagedPointLight(std::shared_ptr<APointLight> PointLight);
};