#include "JohnnieScene.hpp"

#include <Engine/Scene.hpp>
#include <Engine/Camera.hpp>

#include "Gui/JohnnieSystemStatisticsWidget.hpp"
#include "Gui/JohnnieCameraDataWidget.hpp"

void JohnnieScene::OnAwake(void)
{
	InitGui();

	m_Camera = NewActor<AFloatingCamera>(45.0f, (float)(1280 / 720), 0.1f, 100.0f);
    CameraController::Get()->SetViewTarget(m_Camera);

	m_Box = BasicMeshGenerator::CreateBox(glm::vec3(0.15f, 0.15f, 0.15f));
	m_Box->SetLocation(glm::vec3(0.f, 1.f, 0.f));

	m_RoundPlatform = NewActor<AStaticMesh>("Assets/Models/talerz.obj");
}

void JohnnieScene::OnDetach(void)
{
	std::cout << "a";
}

void JohnnieScene::OnTick(double DeltaTime)
{}

void JohnnieScene::InitGui(void)
{
    /** Add widgets */
    m_SystemStatisticsWidget	= NewWidget<WJohnnieSystemStatisticsWidget ,SystemStatisticsAction>();
	m_MainMenuBarWidget			= NewWidget<WJohnnieMainMenuBarWidget, MainMenuBarAction>();
	m_ConsoleLogWidget			= NewWidget<WJohnnieConsoleLogWidget, ConsoleLogAction>();
	m_CameraDataWidget			= NewWidget<WJohnnieCameraDataWidget, CameraDataAction>();

    /** Init widgets actions */
	m_MainMenuBarWidget->BindActionDelegate(MainMenuBarAction::Open,
	[]()
	{
		std::cout << "Open";
	});
}