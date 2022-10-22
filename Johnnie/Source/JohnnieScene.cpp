#include "JohnnieScene.hpp"

#include <Engine/Scene.hpp>
#include <Engine/Camera.hpp>
#include <Engine/Utilities.hpp>

#include "Gui/JohnnieStatisticsWidget.hpp"
#include "Gui/JohnnieCameraWidget.hpp"
#include "Gui/JohnnieSceneWidget.hpp"
#include "Gui/JohnnieViewportWidget.hpp"
#include "JohnnieDelegates.hpp"

void JohnnieScene::OnAwake(void)
{
	m_Camera			= NewActor<AFloatingCamera>(45.0f, (float)(1280 / 720), 0.1f, 100.0f, glm::vec3( -3.550f,2.431f,3.814f ));
	m_PointLight		= NewActor<APointLight>(glm::vec3(0.f, 3.f, 0.f));
	m_DirectionalLight	= NewActor<ADirectionalLight>();

	m_Camera->SetRotation(glm::vec3(0.f,-7.49f,670.62f));

	m_DirectionalLight->SetDirection({ -0.8f,-1.0f,-0.8f });

	CameraController::Get()->SetViewTarget(m_Camera);

	m_Platform = NewActor<AStaticMesh>("Assets/Models/talerz.obj");
	//m_Platform->SetScale(glm::vec3(0.001f, 0.001f, 0.001f));
	m_Platform->SetLocation(glm::vec3(0.f,-0.1f,0.f));
	m_Platform->SetScale(glm::vec3(12.f,1.f,12.f));
	
	//m_Model = BasicMeshGenerator::CreateSphere(1.5f, 25, 25);
	//m_Model->SetMaterialForSlot(0, DefaultMaterials::BasicWhite);
	

	
	SceneDelegates::Get()->OnStaticMeshLoaded.AddLambda([this](AStaticMesh* s) 
	{
			m_Platform->SetMaterialForSlot(0, DefaultMaterials::BasicWhite);
			auto mat = m_Platform->GetMaterialInSlot(0);
			if (mat.has_value())
			{
				mat->get()->SetBaseColor(glm::vec3(0.101f, 0.105f, 0.109f));
				mat->get()->SetSpecular(glm::vec3(0.f));

			}
	});
	InitGui();
	InitLighting();
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
	m_ViewportWidget	= NewWidget<WJohnnieViewportWidget>();

    /** Init widgets actions */

	JohnnieDelegates::Get()->OnBeginLoadingNewModel.AddLambda([this](std::string const& FileName)
	{
		m_Model = NewActor<AStaticMesh>(FileName);
		m_Model->SetScale(glm::vec3(0.02f, 0.02f, 0.02f));
	});
}

void JohnnieScene::InitLighting(void)
{
	m_PointLight->GetData().Ambient = 0.f;

	m_PointLight->SetVisibility(false);
	m_PointLight->SetBoxMeshVisibility(false);

	m_SceneWidget->SetManagedPointLight(m_PointLight);
	m_SceneWidget->SetManagedDirectionalLight(m_DirectionalLight);
}
