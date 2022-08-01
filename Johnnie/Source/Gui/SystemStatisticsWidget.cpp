#include "SystemStatisticsWidget.hpp"

#include <Engine/Gui.hpp>
#include <Engine/Core.hpp>
#include <Engine/System.hpp>

#include <Core/Application/Application.hpp>

void SystemStatisticsWidget::OnRenderGui(void)
{

	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	
	ImGui::Begin("System Stats");
	
	
	ImGui::Text("Frame");
	ImGui::Separator();
	ImGui::Text("Delta Time	: %.2f",		ApplicationStats.DeltaTime);
	ImGui::Text("FPS		   : %.2f",	ApplicationStats.Fps);
	ImGui::Separator(); 
	
	ImGui::Text("Memory Allocations");
	ImGui::Separator();
	ImGui::Text("Currently Allocated : %lx", MemoryStats.CurrentlyAllocated);
	ImGui::Text("Total Allocated : %lx", MemoryStats.TotalAllocated);
	ImGui::Text("Total Allocations : %d", MemoryStats.TotalAllocations);
	ImGui::Text("Total Freed : %lx", MemoryStats.TotalFreed);
	ImGui::Separator(); 

	ImGui::End();
}

void SystemStatisticsWidget::OnTick(void)
{
	UpdateApplicationStats();
	UpdateMemoryStats();
}

void SystemStatisticsWidget::UpdateApplicationStats(void)
{
	using namespace Engine::Core;

	const auto& app = Application::Get();

	ApplicationStats.DeltaTime	= app->GetDT();
	ApplicationStats.Fps		= app->GetFPS();
}

void SystemStatisticsWidget::UpdateMemoryStats(void)
{
	MemoryStats = System::GetMemoryStatistics();
}
