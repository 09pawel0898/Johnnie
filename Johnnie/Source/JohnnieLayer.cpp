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

	SystemVideoBrandingInfo brandingInfo = System::GetGraphicsCardBrandingInfo();
	brandingInfo.Log();

	if (brandingInfo.Brand == GpuBrand::NVIDIA)
	{
		SystemVideoMemoryInfo videoMemInfo = System::GetVideoMemoryInfo();
		auto n = videoMemInfo.Get<SystemNvidiaVideoMemoryInfo>();
		n.value().Log();
	}

	InitGui();
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

void JohnnieLayer::InitGui(void)
{
	m_MainMenuBar.BindToAction(MainMenuBarActions::Open,
	[]()
	{
		std::cout << "Open";
	});
}

void JohnnieLayer::OnRenderGui(void)
{
	m_MainMenuBar.OnRenderGui();
}
