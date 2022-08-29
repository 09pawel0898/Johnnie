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
	SpawnEntitiesToRender();
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
    /** Register tickable widgets manually */
    m_SystemStatisticsWidget = std::make_shared<SystemStatisticsWidget>();
    TickableManager::Get()->RegisterTickable(m_SystemStatisticsWidget);

    m_CameraDataWidget = std::make_shared<CameraDataWidget>();
    TickableManager::Get()->RegisterTickable(m_CameraDataWidget);

    /** Init widgets delegates */
	m_MainMenuBarWidget.BindActionDelegate(MainMenuBarAction::Open,
	[]()
	{
		std::cout << "Open";
	});
}

void JohnnieLayer::SpawnEntitiesToRender(void)
{
	m_StaticMesh = NewObject<OStaticMesh>("Assets/Models/talerz.obj");
	//m_StaticMesh = BasicMeshGenerator::CreateBox(glm::vec3(1.0f, 1.0f, 1.0f));
}

void JohnnieLayer::OnRenderGui(void)
{
	m_MainMenuBarWidget.OnRenderGui();
	m_ConsoleLogWidget.OnRenderGui();
	m_SystemStatisticsWidget->OnRenderGui();
    m_CameraDataWidget->OnRenderGui();

	//static bool bShowDemoWindow = false;
	//ImGui::ShowDemoWindow(&bShowDemoWindow);
}
