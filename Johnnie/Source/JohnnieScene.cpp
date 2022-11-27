#include "JohnnieScene.hpp"

#include <Engine/Camera.hpp>
#include <Engine/Utilities.hpp>

#include "Gui/JohnnieStatisticsWidget.hpp"
#include "Gui/JohnnieCameraWidget.hpp"
#include "Gui/JohnnieSceneWidget.hpp"
#include "Gui/JohnnieViewportWidget.hpp"
#include "Gui/JohnnieAnimatorWidget.hpp"

#include "JohnnieDelegates.hpp"
#include "JohnnieGlobals.hpp"

void JohnnieScene::OnAwake(void)
{
	InitCamera();
	InitScene();
	InitGui();
	InitLighting();
}

void JohnnieScene::OnDetach(void)
{}

void JohnnieScene::OnTick(double DeltaTime)
{	
	UpdatePointLight(DeltaTime);

	static bool Loaded = false;

	if (Input::IsKeyPressed(KeyCode::I) && !Loaded)
	{
		Loaded = true;
		m_SkeletalMesh = NewActor<ASkeletalMesh>("C:\\Users\\PRECISION 3X10\\Desktop\\Johnnie\\Build\\Johnnie\\Assets\\Models\\RumbaDancing.dae");
		m_SkeletalMesh->SetScale(glm::vec3(0.035f, 0.035f, 0.035f));
		m_Animator->SetSkeletalMesh(m_SkeletalMesh);
	}
}

void JohnnieScene::UpdatePointLight(double DeltaTime)
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

	m_AnimatorWidget	= NewWidget<WJohnnieAnimatorWidget>(m_Animator);

    /** Init widgets actions */

	JohnnieDelegates::Get()->OnStaticMeshToLoadPathSelected.AddLambda([this](std::string const& FilePath)
	{
		m_SkeletalMesh = nullptr;
		
		m_StaticMesh = NewActor<AStaticMesh>(FilePath);
		m_StaticMesh->SetScale(glm::vec3(0.035f, 0.035f, 0.035f));
	});
	
	JohnnieDelegates::Get()->OnSkeletalMeshToLoadPathSelected.AddLambda([this](std::string const& FilePath)
	{
		m_StaticMesh = nullptr;
		
		m_SkeletalMesh = NewActor<ASkeletalMesh>(FilePath);
		m_SkeletalMesh->SetScale(glm::vec3(0.035f, 0.035f, 0.035f));

		m_Animator->ClearAnimations();
		m_Animator->SetSkeletalMesh(m_SkeletalMesh);
	});	
	
	JohnnieDelegates::Get()->OnAnimationToLoadPathSelected.AddLambda([this](std::string const& FilePath)
	{
		Check(m_Animator);
		m_Animator->AsyncImportSingleAnimationFromFile(FilePath,true);
	});
}

void JohnnieScene::InitLighting(void)
{
	m_PointLight		= NewActor<APointLight>(glm::vec3(0.f, 3.f, 0.f));
	m_DirectionalLight	= NewActor<ADirectionalLight>();

	m_DirectionalLight->SetDirection({ -0.8f,-1.0f,-0.8f });

	m_PointLight->GetData().Ambient = 0.f;

	m_PointLight->SetVisibility(false);
	m_PointLight->SetBoxMeshVisibility(false);

	m_SceneWidget->SetManagedPointLight(m_PointLight);
	m_SceneWidget->SetManagedDirectionalLight(m_DirectionalLight);
}

void JohnnieScene::InitCamera(void)
{
	m_Camera = NewActor<AFloatingCamera>(45.0f, (float)(1280 / 720), 0.1f, 100.0f, glm::vec3(-3.550f, 2.431f, 3.814f));

	m_Camera->SetRotation(glm::vec3(0.f, -16.29f, 653.89f));
	m_Camera->SetLocation(glm::vec3(-3.95f,5.88f, 8.52f));

	CameraController::Get()->SetViewTarget(m_Camera);
}

void JohnnieScene::InitScene(void)
{
	m_Platform = NewActor<AStaticMesh>(JohnnieGlobals::PlatformModelFilePath, OnStaticMeshAsyncLoadingFinishedDelegate::CreateRaw(this,&JohnnieScene::InitPlatformMaterial));
	
	m_Platform->SetLocation(glm::vec3(0.f, -0.1f, 0.f));
	m_Platform->SetScale(glm::vec3(20.f, 1.f, 20.f));

	m_Animator = NewObject<OAnimator>();
}

void JohnnieScene::InitPlatformMaterial(AStaticMesh* Platform)
{
	if (Material* material = Platform->GetMaterialInSlot(0))
	{
		material->SetBaseColor(glm::vec3(0.101f, 0.105f, 0.109f));
		//material->SetBaseColor(glm::vec3(0.8f, 0.105f, 0.109f));
		material->SetSpecular(glm::vec3(0.f));
	}
}
