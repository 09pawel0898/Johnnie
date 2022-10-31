#pragma once

#include <Engine/CoreMinimal.hpp>
#include <Engine/Scene.hpp>

#include "Gui/JohnnieMainMenuBarWidget.hpp"
#include "Gui/JohnnieLoggerWidget.hpp"

class WJohnnieMainMenuBarWidget;
class WJohnnieCameraWidget;
class WJohnnieStatisticsWidget;
class WJohnnieSceneWidget;
class WJohnnieViewportWidget;

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
	TSharedPtr<WJohnnieMainMenuBarWidget>	m_MainMenuBarWidget;
	TSharedPtr<WJohnnieLoggerWidget>		m_LoggerWidget;
	TSharedPtr<WJohnnieStatisticsWidget>	m_StatisticsWidget;
	TSharedPtr<WJohnnieCameraWidget>		m_CameraDataWidget;
	TSharedPtr<WJohnnieSceneWidget>			m_SceneWidget;
	TSharedPtr<WJohnnieViewportWidget>		m_ViewportWidget;

	/** Floating Camera */
	TSharedPtr<AFloatingCamera>				m_Camera;

	/** Entities */
	TSharedPtr<AStaticMesh>					m_Model;
	TSharedPtr<AStaticMesh>					m_Platform;

	TSharedPtr<AStaticMesh>					m_Sphere;
	TSharedPtr<APointLight>					m_PointLight;
	TSharedPtr<ADirectionalLight>			m_DirectionalLight;

private:
	/** Layer Interface Impl*/
	virtual void OnAwake(void) override;
	virtual void OnDetach(void) override;

	virtual void OnTick(double DeltaTime) override;

private:
	void InitGui(void);
	void InitLighting(void);
};