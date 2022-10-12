#pragma once

#include <Engine/CoreMinimal.hpp>
#include <Engine/Scene.hpp>

#include "Gui/JohnnieMainMenuBarWidget.hpp"
#include "Gui/JohnnieLoggerWidget.hpp"

class WJohnnieMainMenuBarWidget;
class WJohnnieCameraWidget;
class WJohnnieStatisticsWidget;
class WJohnnieSceneWidget;

namespace Engine
{
	class AFloatingCamera;
	class AStaticMesh;
}

class JohnnieScene final : public Scene
{
public:
	JohnnieScene() = default;

private:
	/** UI Widgets */
	std::shared_ptr<WJohnnieMainMenuBarWidget>	m_MainMenuBarWidget;
	std::shared_ptr<WJohnnieLoggerWidget>		m_LoggerWidget;
	std::shared_ptr<WJohnnieStatisticsWidget>	m_StatisticsWidget;
	std::shared_ptr<WJohnnieCameraWidget>		m_CameraDataWidget;
	std::shared_ptr<WJohnnieSceneWidget>		m_SceneWidget;

	/** Floating Camera */
	std::shared_ptr<AFloatingCamera>					m_Camera;

	/** Entities */
	std::shared_ptr<AStaticMesh>						m_Model = nullptr;

	std::shared_ptr<AStaticMesh>						m_Sphere			= nullptr;
	std::shared_ptr<APointLight>						m_PointLight		= nullptr;
	std::shared_ptr<ADirectionalLight>					m_DirectionalLight	= nullptr;

private:
	/** Layer Interface Impl*/
	virtual void OnAwake(void) override;
	virtual void OnDetach(void) override;

	virtual void OnTick(double DeltaTime) override;

private:
	void InitGui(void);
};