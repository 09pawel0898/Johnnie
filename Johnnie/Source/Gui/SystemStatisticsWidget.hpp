#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"

#include <System/MemoryManager.hpp>
#include <System/System.hpp>


enum class SystemStatisticsAction : uint8_t
{};

class SystemStatisticsWidget final : public ImGuiWidgetBase<SystemStatisticsAction>
{
private:
	struct ApplicationStats
	{
		double DeltaTime;
		double Fps;
	};

	ApplicationStats		ApplicationStats;
	MemoryStatistics		MemoryStats;
	SystemVideoBrandingInfo GraphicsCardInfo;
	SystemMemoryInfo		MemoryInfo;
	SystemVideoMemoryInfo	VideoMemoryInfo;

public:
	SystemStatisticsWidget();

	void InitStaticStats(void);

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;

private:
	void UpdateApplicationStats(void);
	void UpdateMemoryAllocationStats(void);
	void UpdateMemoryStats(void);
	void UpdateVideoMemoryStats(void);
};