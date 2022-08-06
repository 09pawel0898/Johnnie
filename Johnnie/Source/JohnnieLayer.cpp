#include "JohnnieLayer.hpp"

#include <Engine/Gui.hpp>
#include <Engine/System.hpp>
#include <Engine/Utilities.hpp>

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
{
	/** TO DO : Implement tickable widgets with fixed interval **/
	{
		static Utility::TimePoint lastStatsUpdTime = Utility::Time::now();
		Utility::TimePoint timeNow = Utility::Time::now();
		Utility::Duration timeAfterLastUpd = timeNow - lastStatsUpdTime;

		if (timeAfterLastUpd.count() > .1f)
		{
			m_SystemStatistics.OnTick();
			lastStatsUpdTime = Utility::Time::now();
		}
	}
}

void JohnnieLayer::OnEvent(Events::Event& Event)
{}

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
	m_SystemStatistics.OnRenderGui();

	static bool bShowDemoWindow = true;
	ImGui::ShowDemoWindow(&bShowDemoWindow);
}
