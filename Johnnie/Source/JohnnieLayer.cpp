#include "JohnnieLayer.hpp"

#include <Engine/Gui.hpp>
#include <Engine/System.hpp>
#include <Engine/Utilities.hpp>
#include <Engine/Renderer.hpp>
#include <Engine/CoreObject.hpp>
#include <Engine/Camera.hpp>
#include <Scene/Entities/Mesh/StaticMesh.hpp>
#include <Scene/Entities/Mesh/BasicMeshGenerator.hpp>

#include "Gui/SystemStatisticsWidget.hpp"
#include "Gui/CameraDataWidget.hpp"

JohnnieLayer::JohnnieLayer(std::string_view Name) noexcept
	:	Layer(Name)
{
	m_Camera = NewObject<OFloatingCamera>(45.0f, (float)(1280 / 720), 0.1f, 100.0f);
}

void JohnnieLayer::OnAwake(void)
{
	InitGui();

    CameraController::Get()->SetViewTarget(m_Camera);

	m_StaticMesh = NewObject<OStaticMesh>("Assets/Models/talerz.obj");
}

void JohnnieLayer::OnDetach(void)
{}

void JohnnieLayer::OnRender(void) const
{
    static std::shared_ptr<RHIShader> staticMeshShader = RHI::RHIShader::Create("MeshShader", "Assets/Shaders/mesh_shader.glsl");
  
    m_StaticMesh->Draw(staticMeshShader);
}

void JohnnieLayer::OnTick(double DeltaTime)
{}

void JohnnieLayer::OnEvent(Events::Event& Event)
{}

void JohnnieLayer::InitGui(void)
{
    /** Add widgets */
    m_SystemStatisticsWidget	= NewWidget<WSystemStatisticsWidget ,SystemStatisticsAction>();
	m_MainMenuBarWidget			= NewWidget<WMainMenuBarWidget, MainMenuBarAction>();
	m_ConsoleLogWidget			= NewWidget<WConsoleLogWidget, ConsoleLogAction>();
	m_CameraDataWidget			= NewWidget<WCameraDataWidget, CameraDataAction>();

    /** Init widgets actions */
	
	m_MainMenuBarWidget->BindActionDelegate(MainMenuBarAction::Open,
	[]()
	{
		std::cout << "Open";
	});
}

void JohnnieLayer::OnRenderGui(void)
{
	//m_MainMenuBarWidget.OnRenderGui();
	//m_ConsoleLogWidget.OnRenderGui();
	m_SystemStatisticsWidget->OnRenderGui();
   //m_CameraDataWidget->OnRenderGui();

	//static bool bShowDemoWindow = false;
	//ImGui::ShowDemoWindow(&bShowDemoWindow);
}
