#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"

#include <System/MemoryManager.hpp>

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

	ApplicationStats			ApplicationStats;
	Engine::MemoryStatistics	MemoryStats;


public:
	virtual void OnRenderGui(void) override;
	virtual void OnTick(void) override;

private:
	void UpdateApplicationStats(void);
	void UpdateMemoryStats(void);
};