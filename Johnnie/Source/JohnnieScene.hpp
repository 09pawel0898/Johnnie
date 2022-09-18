#pragma once

#include <Engine/CoreMinimal.hpp>
#include <Engine/Scene.hpp>

#include "Gui/JohnnieMainMenuBarWidget.hpp"
#include "Gui/JohnnieConsoleLogWidget.hpp"

class WJohnnieMainMenuBarWidget;
class WJohnnieConsoleLogWidget;
class WJohnnieCameraDataWidget;
class WJohnnieSystemStatisticsWidget;

namespace Engine
{
	class AFloatingCamera;
	class AStaticMesh;
	class APointLight;
}

class JohnnieScene final : public Scene
{
public:
	JohnnieScene() = default;

private:
	/** UI Widgets */
	std::shared_ptr<WJohnnieMainMenuBarWidget>			m_MainMenuBarWidget;
	std::shared_ptr<WJohnnieConsoleLogWidget>			m_ConsoleLogWidget;
	std::shared_ptr<WJohnnieSystemStatisticsWidget>		m_SystemStatisticsWidget;
	std::shared_ptr<WJohnnieCameraDataWidget>			m_CameraDataWidget;	

	/** Floating Camera */
	std::shared_ptr<AFloatingCamera>					m_Camera;

	/** Entities */
	std::shared_ptr<AStaticMesh>						m_RoundPlatform = nullptr;
	std::shared_ptr<APointLight>						m_PointLight	= nullptr;

private:
	/** Layer Interface Impl*/
	virtual void OnAwake(void) override;
	virtual void OnDetach(void) override;

	virtual void OnTick(double DeltaTime) override;

private:
	void InitGui(void);
};