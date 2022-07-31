#include "JohnnieLayer.hpp"

#include <Engine/Gui.hpp>
#include <Engine/System.hpp>

JohnnieLayer::JohnnieLayer(std::string_view Name) noexcept
	:	Layer(Name)
{
}

void JohnnieLayer::OnAwake(void)
{
	auto memoryInfo = System::GetMemoryInfo();
	Check(memoryInfo);

	memoryInfo.value().Log();

	const auto& memoryStats = System::GetMemoryStatistics();
	
	memoryStats.Log();


}

void JohnnieLayer::OnDetach(void)
{
}

void JohnnieLayer::OnRender(void) const
{
}

void JohnnieLayer::OnTick(double DeltaTime)
{
}

void JohnnieLayer::OnEvent(Events::Event& Event)
{
}

void JohnnieLayer::OnRenderGui(void)
{
	Gui::Begin("MyWindow");
	Gui::Text("Hello there");
	Gui::Text("Hello there");
	Gui::Text("Hello there");
	Gui::End();

	if (bool demoWindow = true; demoWindow)
	{
		ImGui::ShowDemoWindow(&demoWindow);
	}
}
