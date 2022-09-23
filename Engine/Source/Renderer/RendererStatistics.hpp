#pragma once

namespace Engine
{
	struct MeshStatistics
	{
		uint32_t TrisCount;
	};

	struct RendererStatistics
	{
		uint32_t DrawCalls;
		uint32_t TotalTrisCount;
		
		double FrameDuration;
		double TickDuration;
		double RenderingTime;
	};
}