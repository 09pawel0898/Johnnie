#pragma once

namespace Engine
{
	struct MeshStatistics
	{
		uint32_t TrisCount{ 0 };
	};

	struct RendererStatistics
	{
		uint32_t DrawCalls;
		uint32_t TotalTrisCount;
		
		/** In Milliseconds */
		double FrameDuration;
		double TickDuration;
		double RenderDuration;
		double RenderWorldDuration;
		double RenderGUIDuration;

		uint32_t MeshesCount;
		uint32_t ActorsCount;
	};
}