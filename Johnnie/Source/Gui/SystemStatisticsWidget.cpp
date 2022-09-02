#include "SystemStatisticsWidget.hpp"

#include <Engine/Gui.hpp>
#include <Engine/Core.hpp>
#include <Engine/System.hpp>
#include <Engine/Utilities.hpp>
#include <Engine/Debug.hpp>

#include <Core/Application/Application.hpp>

#include <inttypes.h>

WSystemStatisticsWidget::WSystemStatisticsWidget()
{
	InitStaticStats();

	SetTickInterval(0.1f);
}

void WSystemStatisticsWidget::InitStaticStats(void)
{
	GraphicsCardInfo = System::GetGraphicsCardBrandingInfo();
}

void WSystemStatisticsWidget::OnRenderGui(void)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	
	ImGui::Begin("System Stats");
	{
		{
			ImGui::Text("Frame");
			ImGui::Separator();
			ImGui::Text("Delta Time    : %.4f ms",	ApplicationStats.DeltaTime * 1000.0f);
			ImGui::Text("FPS		   : %.2f",		ApplicationStats.Fps);
			ImGui::Separator(); 
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
		}

		{
			ImGui::Text("Memory Allocations");
			ImGui::Separator();
			ImGui::Text("Currently Allocated : %s",	Utility::BytesToString(MemoryStats.CurrentlyAllocated).c_str());
			ImGui::Text("Total Allocated : %s",		Utility::BytesToString(MemoryStats.TotalAllocated).c_str());
			ImGui::Text("Total Allocations : %d",	MemoryStats.TotalAllocations);
			ImGui::Text("Total Freed : %s ",		Utility::BytesToString(MemoryStats.TotalFreed).c_str());
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
		}

		{
			ImGui::Text("Memory Statistics");
			ImGui::Separator();
			ImGui::Text("Physical Memory (Available/Total)");
			ImGui::Text("%s / %s",	Utility::BytesToString(MemoryInfo.AvailablePhysMemory).c_str(), 
									Utility::BytesToString(MemoryInfo.TotalPhysMemory).c_str());

			ImGui::Text("Virtual Memory (Available/Total)");
			ImGui::Text("%s / %s",	Utility::BytesToString(MemoryInfo.AvailableVirtualMemory).c_str(), 
									Utility::BytesToString(MemoryInfo.TotalVirtualMemory).c_str());
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
		}

		{
			ImGui::Text("Graphics Card");
			ImGui::Separator();
			ImGui::Text("Brand : %s", GraphicsCardInfo.Vendor.c_str());
	
			ImGui::Text("Model : %s", GraphicsCardInfo.Renderer.c_str());
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
		}
		
		{
			if (GraphicsCardInfo.Brand == GpuBrand::NVIDIA)
			{
				auto NvidiaVideoMemInfo = VideoMemoryInfo.Get<SystemNvidiaVideoMemoryInfo>();
				if (NvidiaVideoMemInfo)
				{
					SystemNvidiaVideoMemoryInfo videoInfo = NvidiaVideoMemInfo.value();
					ImGui::Text("Video Memory");

					ImGui::Separator();
					ImGui::Text("Dedicated Video Memory : %s",	Utility::BytesToString(1024 * videoInfo.DedicatedVideoMemory).c_str());
					ImGui::Text("Current Available : %s",		Utility::BytesToString(1024 * videoInfo.CurrentAvailableVideoMemory).c_str());
					ImGui::Text("Total Available : %s",			Utility::BytesToString(1024 * videoInfo.TotalAvailableVideoMemory).c_str());
					ImGui::Text("Evicted Count : %d",			videoInfo.EvictedCount);
					ImGui::Text("Evicted Size: %s",				Utility::BytesToString(1024 * videoInfo.EvictedSize).c_str());
					ImGui::Separator();
				}

			}
			else if (GraphicsCardInfo.Brand == GpuBrand::AMD)
			{
				auto AMDVideoMemInfo = VideoMemoryInfo.Get<SystemAMDVideoMemoryInfo>();
				if (AMDVideoMemInfo)
				{
					SystemAMDVideoMemoryInfo videoInfo = AMDVideoMemInfo.value();
					ImGui::Text("Video Memory");

					ImGui::Separator();
					ImGui::Text("Current Available : % s",		Utility::BytesToString(1024 * videoInfo.TotalPool).c_str());
					ImGui::Text("Total Available : %s",			Utility::BytesToString(1024 * videoInfo.LargestBlock).c_str());
					ImGui::Text("Total Aux Free : %s",			Utility::BytesToString(1024 * videoInfo.TotalAux).c_str());
					ImGui::Text("Largest Aux Free Block : %d",	Utility::BytesToString(1024 * videoInfo.LargestAux).c_str());
					ImGui::Separator();
				}
			}
		}
	}
	ImGui::End();
}

void WSystemStatisticsWidget::OnTick(double DeltaTime)
{
	UpdateApplicationStats();
	UpdateMemoryAllocationStats();
	UpdateMemoryStats();
	UpdateVideoMemoryStats();
}

void WSystemStatisticsWidget::UpdateApplicationStats(void)
{
	using namespace Engine::Core;

	const auto& app = Application::Get();

	ApplicationStats.DeltaTime	= app->GetDT();
	ApplicationStats.Fps		= app->GetFPS();
}

void WSystemStatisticsWidget::UpdateMemoryAllocationStats(void)
{
	MemoryStats = System::GetMemoryStatistics();
}

void WSystemStatisticsWidget::UpdateMemoryStats(void)
{
	if (auto memoryInfo = System::GetMemoryInfo())
	{
		MemoryInfo = memoryInfo.value();
	}
}

void WSystemStatisticsWidget::UpdateVideoMemoryStats(void)
{
	static bool bSupported = true;

	if(bSupported)
	{
		VideoMemoryInfo = System::GetVideoMemoryInfo();
		if (!VideoMemoryInfo.IsValid())
		{
			bSupported = false;
		}
	}
}
