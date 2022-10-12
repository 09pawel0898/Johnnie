#include "JohnnieScene.hpp"

#include <Engine/Scene.hpp>
#include <Engine/Camera.hpp>
#include <Engine/Utilities.hpp>

#include "Gui/JohnnieStatisticsWidget.hpp"
#include "Gui/JohnnieCameraWidget.hpp"
#include "Gui/JohnnieSceneWidget.hpp"
#include "JohnnieDelegates.hpp"

void JohnnieScene::OnAwake(void)
{
	InitGui();

	m_Camera			= NewActor<AFloatingCamera>(45.0f, (float)(1280 / 720), 0.1f, 100.0f);	
	m_PointLight		= NewActor<APointLight>(glm::vec3(0.f, 3.f, 0.f));
	m_DirectionalLight	= NewActor<ADirectionalLight>();

	CameraController::Get()->SetViewTarget(m_Camera);

	m_Model = BasicMeshGenerator::CreateSphere(1.5f, 200, 200);
	m_Model->SetMaterialForSlot(0, DefaultMaterials::BasicWhite);

	m_SceneWidget->SetManagedPointLight(m_PointLight);
}

void JohnnieScene::OnDetach(void)
{}

void JohnnieScene::OnTick(double DeltaTime)
{	
	static double step = 0.0;
	step += DeltaTime;
	
	float lightX = (float)(3.0f * sin(step));
	float lightY = 2.f;
	float lightZ = (float)(1.5f * cos(step));
	glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);
	
	m_PointLight->SetLocation(lightPos);
}

void JohnnieScene::InitGui(void)
{
    /** Add widgets */
    m_StatisticsWidget	= NewWidget<WJohnnieStatisticsWidget>();
	m_MainMenuBarWidget	= NewWidget<WJohnnieMainMenuBarWidget>();
	m_LoggerWidget		= NewWidget<WJohnnieLoggerWidget>();
	m_CameraDataWidget	= NewWidget<WJohnnieCameraWidget>();
	m_SceneWidget		= NewWidget<WJohnnieSceneWidget>();

    /** Init widgets actions */

	JohnnieDelegates::Get()->OnBeginLoadingNewModel.AddLambda([this](std::string const& FileName)
	{
		m_Model = NewActor<AStaticMesh>(FileName);
		m_Model->SetScale(glm::vec3(0.02f, 0.02f, 0.02f));
	});
}