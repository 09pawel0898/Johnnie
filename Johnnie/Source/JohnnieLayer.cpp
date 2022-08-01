#include "JohnnieLayer.hpp"

#include <Engine/Gui.hpp>
#include <Engine/System.hpp>

JohnnieLayer::JohnnieLayer(std::string_view Name) noexcept
	:	Layer(Name)
{}

void JohnnieLayer::OnAwake(void)
{
	InitGui();
}

void JohnnieLayer::OnDetach(void)
{}

void JohnnieLayer::OnRender(void) const
{}

void JohnnieLayer::OnTick(double DeltaTime)
{}

void JohnnieLayer::OnEvent(Events::Event& Event)
{}

static void func(int x)
{
	std::cout << "s";
}

void JohnnieLayer::InitGui(void)
{
	m_MainMenuBar.BindActionDelegate(MainMenuBarAction::Open,
	[]()
	{
		std::cout << "Open";
	});
}



void JohnnieLayer::OnRenderGui(void)
{
	m_MainMenuBar.OnRenderGui();
	m_ConsoleLog.OnRenderGui();

	static bool bShowDemoWindow = true;
	ImGui::ShowDemoWindow(&bShowDemoWindow);
}
