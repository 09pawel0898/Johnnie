#pragma once

#include <Engine/CoreMinimal.hpp>
#include <Engine/Layers.hpp>

#include "Gui/MainMenuBarWidget.hpp"
#include "Gui/ConsoleLogWidget.hpp"
#include "Gui/SystemStatisticsWidget.hpp"

namespace Engine
{
	class OFloatingCamera;
}

class JohnnieLayer final : public Engine::Layer
{
public:
	explicit JohnnieLayer(std::string_view Name) noexcept;

private:
	/** UI Widgets */
	MainMenuBarWidget		m_MainMenuBar;
	ConsoleLogWidget		m_ConsoleLog;
	SystemStatisticsWidget	m_SystemStatistics;

	/** Floating Camera */
	std::shared_ptr<OFloatingCamera> m_Camera;

private:
	/** Layer Interface Impl*/
	virtual void OnAwake(void) override;
	virtual void OnDetach(void) override;

	virtual void OnTick(double DeltaTime) override;
	virtual void OnEvent(Events::Event& Event) override;
	
	virtual void OnRender(void) const override;
	virtual void OnRenderGui(void) override;

private:
	void InitGui(void);
};