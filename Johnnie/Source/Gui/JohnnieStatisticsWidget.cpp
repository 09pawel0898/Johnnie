#include "JohnnieStatisticsWidget.hpp"

#include <Engine/Gui.hpp>
#include <Engine/System.hpp>
#include <Engine/Utilities.hpp>
#include <Engine/Debug.hpp>

#include <Core/Application/Application.hpp>

#include <inttypes.h>

WJohnnieStatisticsWidget::WJohnnieStatisticsWidget()
{
	InitStaticStats();

	SetTickInterval(0.1f);
}

void WJohnnieStatisticsWidget::InitStaticStats(void)
{
	GraphicsCardInfo = System::GetGraphicsCardBrandingInfo();
}

void WJohnnieStatisticsWidget::OnRenderGui(void)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	

	ImGui::Begin("Statistics");
	{
		if (ImGui::CollapsingHeader("System"))
		{
			RenderSystemStats();
		}
		if (ImGui::CollapsingHeader("Renderer"))
		{
			RenderRendererStats();
		}

		if (m_bRenderMeshStats)
		{
			if(ImGui::CollapsingHeader("Static Mesh"))
			{
				RenderMeshStats();
			}
		}
	}
	ImGui::End();
}

void WJohnnieStatisticsWidget::OnTick(double DeltaTime)
{
	UpdateApplicationStats();
	UpdateMemoryAllocationStats();
	UpdateMemoryStats();
	UpdateVideoMemoryStats();
	UpdateRendererStats();
}

void WJohnnieStatisticsWidget::SetMeshStats(MeshStatistics&& MeshStatistics)
{
	MeshStats = std::move(MeshStatistics);
	SetRenderMeshStats(true);
}

void WJohnnieStatisticsWidget::RenderSystemStats(void)
{
	{
		ImGui::Text("Memory Allocations");
		ImGui::Separator();
		ImGui::Text("Currently Allocated : %s", Utility::BytesToString(MemoryStats.CurrentlyAllocated).c_str());
		ImGui::Text("Total Allocated : %s", Utility::BytesToString(MemoryStats.TotalAllocated).c_str());
		ImGui::Text("Total Allocations : %d", MemoryStats.TotalAllocations);
		ImGui::Text("Total Freed : %s ", Utility::BytesToString(MemoryStats.TotalFreed).c_str());
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 20.0f));
	}

	{
		ImGui::Text("Memory Statistics");
		ImGui::Separator();
		ImGui::Text("Physical Memory (Available/Total)");
		ImGui::Text("%s / %s", Utility::BytesToString(MemoryInfo.AvailablePhysMemory).c_str(),
			Utility::BytesToString(MemoryInfo.TotalPhysMemory).c_str());

		ImGui::Text("Virtual Memory (Available/Total)");
		ImGui::Text("%s / %s", Utility::BytesToString(MemoryInfo.AvailableVirtualMemory).c_str(),
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
				ImGui::Text("Dedicated Video Memory : %s", Utility::BytesToString(1024 * videoInfo.DedicatedVideoMemory).c_str());
				ImGui::Text("Current Available : %s", Utility::BytesToString(1024 * videoInfo.CurrentAvailableVideoMemory).c_str());
				ImGui::Text("Total Available : %s", Utility::BytesToString(1024 * videoInfo.TotalAvailableVideoMemory).c_str());
				ImGui::Text("Evicted Count : %d", videoInfo.EvictedCount);
				ImGui::Text("Evicted Size: %s", Utility::BytesToString(1024 * videoInfo.EvictedSize).c_str());
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
				ImGui::Text("Current Available : % s", Utility::BytesToString(1024 * videoInfo.TotalPool).c_str());
				ImGui::Text("Total Available : %s", Utility::BytesToString(1024 * videoInfo.LargestBlock).c_str());
				ImGui::Text("Total Aux Free : %s", Utility::BytesToString(1024 * videoInfo.TotalAux).c_str());
				ImGui::Text("Largest Aux Free Block : %d", Utility::BytesToString(1024 * videoInfo.LargestAux).c_str());
				ImGui::Separator();
			}
		}
	}
}

void WJohnnieStatisticsWidget::RenderRendererStats(void)
{
	{
		ImGui::Text("General");
		ImGui::Separator();

		ImGui::Text("FPS		   : %.2f",		ApplicationStats.Fps);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 20.0f));
	}
	
	{
		ImGui::Text("Renderer Statistics");
		ImGui::Separator();
		ImGui::Text("Delta Time       : %.4f ms",	ApplicationStats.DeltaTime * 1000.0f);
		ImGui::Text("Frame			: %.5f ms",		RendererStats.FrameDuration);
		ImGui::Text("Tick			 : %.5f ms", RendererStats.TickDuration);
		ImGui::Text("RenderOverall	: %.5f ms", RendererStats.RenderDuration);
		ImGui::Text("RenderWorld      : %.5f ms", RendererStats.RenderWorldDuration);
		ImGui::Text("RenderGui		: %.5f ms", RendererStats.RenderGUIDuration);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 20.0f));
	}

}

void WJohnnieStatisticsWidget::RenderMeshStats(void)
{
	{
		ImGui::Text("Mesh Info");
		ImGui::Separator();

		ImGui::Text("Faces :	%d", MeshStats.TrisCount);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 20.0f));
	}
}

void WJohnnieStatisticsWidget::UpdateApplicationStats(void)
{
	using namespace Engine::Core;

	const auto& app = Application::Get();

	ApplicationStats.DeltaTime	= app->GetDT();
	ApplicationStats.Fps		= app->GetFPS();
}

void WJohnnieStatisticsWidget::UpdateMemoryAllocationStats(void)
{
	MemoryStats = System::GetMemoryStatistics();
}

void WJohnnieStatisticsWidget::UpdateMemoryStats(void)
{
	if (auto memoryInfo = System::GetMemoryInfo())
	{
		MemoryInfo = memoryInfo.value();
	}
}

void WJohnnieStatisticsWidget::UpdateVideoMemoryStats(void)
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

void WJohnnieStatisticsWidget::UpdateRendererStats(void)
{
	RendererStatistics const& rendererStats = Renderer::GetRendererStats();
	RendererStats = rendererStats;
}
