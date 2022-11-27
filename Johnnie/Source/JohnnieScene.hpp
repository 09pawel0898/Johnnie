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
class WJohnnieAnimatorWidget;

namespace Engine
{
	class AFloatingCamera;
	class AStaticMesh;
	class ASkeletalMesh;
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
	TSharedPtr<WJohnnieAnimatorWidget>		m_AnimatorWidget;

private:
	/** Floating Camera */
	TSharedPtr<AFloatingCamera>				m_Camera;

	/** Entities */
	TSharedPtr<AStaticMesh>					m_StaticMesh;
	TSharedPtr<ASkeletalMesh>				m_SkeletalMesh;

	TSharedPtr<AStaticMesh>					m_Platform;

	TSharedPtr<AStaticMesh>					m_Sphere;
	TSharedPtr<APointLight>					m_PointLight;
	TSharedPtr<ADirectionalLight>			m_DirectionalLight;

private:
	TSharedPtr<OAnimator>					m_Animator;

private:
	/** Scene Interface Impl*/
	virtual void OnAwake(void) override;
	virtual void OnDetach(void) override;

	virtual void OnTick(double DeltaTime) override;

private:
	void InitGui(void);
	void InitCamera(void);
	void InitScene(void);
	void InitLighting(void);

	void InitPlatformMaterial(AStaticMesh* Platform);

	void UpdatePointLight(double DeltaTime);
	void Test(ASkeletalMesh* SkeletalMesh);
};