#include "EnginePCH.hpp"

#include "RendererStatistics.hpp"
#include "Core/Debug/ProfileMacros.hpp"

namespace Engine
{
    void RendererStatistics::ResetStats(void)
    {
        MeshesCount = 0;
        DrawCalls = 0;
        TotalTrisCount = 0;
    }

    void RendererStatistics::UpdateStats(void)
    {
        FrameDuration = (double)(GET_PROFILE_RESULT("RendererStats_FrameDuration") / 1000.0);
        TickDuration = (double)(GET_PROFILE_RESULT("RendererStats_TickDuration") / 1000.0);
        RenderDuration = (double)(GET_PROFILE_RESULT("RendererStats_RenderDuration") / 1000.0);
        RenderWorldDuration = (double)(GET_PROFILE_RESULT("RendererStats_RenderWorldDuration") / 1000.0);
        RenderGUIDuration = (double)(GET_PROFILE_RESULT("RendererStats_RenderGUIDuration") / 1000.0);
    }
}