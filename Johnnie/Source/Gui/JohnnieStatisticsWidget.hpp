#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"

#include <System/MemoryManager.hpp>
#include <System/System.hpp>
#include <Engine/Renderer.hpp>

enum class StatisticsAction : uint8_t
{};

class WJohnnieStatisticsWidget final : public ImGuiWidgetBase<StatisticsAction>
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

	RendererStatistics		RendererStats;
	MeshStatistics			MeshStats;
	bool m_bRenderMeshStats;

public:
	WJohnnieStatisticsWidget();

	void InitStaticStats(void);

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;

	void SetMeshStats(MeshStatistics&& MeshStatistics);
	void SetRenderMeshStats(bool ShouldRender);

private:
	void RenderSystemStats(void);
	void RenderRendererStats(void);
	void RenderMeshStats(void);

	void UpdateApplicationStats(void);
	void UpdateMemoryAllocationStats(void);
	void UpdateMemoryStats(void);
	void UpdateVideoMemoryStats(void);
	void UpdateRendererStats(void);
};

FORCEINLINE void WJohnnieStatisticsWidget::SetRenderMeshStats(bool ShouldRender)
{
	m_bRenderMeshStats = ShouldRender;
}