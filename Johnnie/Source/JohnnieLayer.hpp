#pragma once

#include <Engine/CoreMinimal.hpp>
#include <Engine/Layers.hpp>

#include "Gui/MainMenuBarWidget.hpp"
#include "Gui/ConsoleLogWidget.hpp"

class WMainMenuBarWidget;
class WConsoleLogWidget;
class WCameraDataWidget;
class WSystemStatisticsWidget;

namespace Engine
{
	class OFloatingCamera;
	class OStaticMesh;
}

class JohnnieLayer final : public Engine::Layer
{
public:
	explicit JohnnieLayer(std::string_view Name) noexcept;

private:
	/** UI Widgets */
	std::shared_ptr<WMainMenuBarWidget>			m_MainMenuBarWidget;
	std::shared_ptr<WConsoleLogWidget>			m_ConsoleLogWidget;
	std::shared_ptr<WSystemStatisticsWidget>	m_SystemStatisticsWidget;
	std::shared_ptr<WCameraDataWidget>			m_CameraDataWidget;	

	/** Floating Camera */
	std::shared_ptr<OFloatingCamera> m_Camera;

	/** Entities */
	std::shared_ptr<OStaticMesh> m_StaticMesh;

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