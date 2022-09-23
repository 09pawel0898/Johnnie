#include "JohnnieScene.hpp"

#include <Engine/Scene.hpp>
#include <Engine/Camera.hpp>
#include <Engine/Utilities.hpp>

#include "Gui/JohnnieSystemStatisticsWidget.hpp"
#include "Gui/JohnnieCameraDataWidget.hpp"

void JohnnieScene::OnAwake(void)
{
	InitGui();

	m_Camera		= NewActor<AFloatingCamera>(45.0f, (float)(1280 / 720), 0.1f, 100.0f);

	CameraController::Get()->SetViewTarget(m_Camera);
	
	m_PointLight	= NewActor<APointLight>(glm::vec3(0.f, 3.f, 0.f));
	m_RoundPlatform = NewActor<AStaticMesh>("Assets/Models/talerz.obj");

	//int16_t X = 10, Y = 8;
	//m_Clones.reserve(X*Y);
	//for (int16_t i = 0; i < X; i++)
	//{
	//	for (int16_t j = 0; j < Y; j++)
	//	{
	//		m_Clones.emplace_back(m_RoundPlatform->Clone());
	//		m_Clones[(int32_t)(i * Y + j)]->SetLocation(glm::vec3(i * 5.0f, 0.f, j * 3.0f));
	//	}
	//}



	//m_RoundPlatform->SetMaterialForSlot(2,DefaultMaterials::BasicWhite);
	
	//std::shared_ptr<Material> platformMaterial = std::make_shared<Material>();
	//platformMaterial->SetBaseColor()
	// 
	//m_Sphere		= BasicMeshGenerator::CreatSphere(1, 100, 100);
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