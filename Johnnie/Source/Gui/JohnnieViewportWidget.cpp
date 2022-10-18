#include "JohnnieViewportWidget.hpp"

#include <Engine/Renderer.hpp>
#include <Engine/Camera.hpp>

static FORCEINLINE bool operator!=(ImVec2 const& Lhs, ImVec2 const& Rhs)
{
    return !(Lhs.x == Rhs.x && Lhs.y == Rhs.y);
}

WJohnnieViewportWidget::WJohnnieViewportWidget()
{
	SetTickEnabled(false);
}

void WJohnnieViewportWidget::OnRenderGui(void)
{
    ImGui::Begin("Viewport");

    m_ViewportSize = ImGui::GetWindowSize();
    if (m_ViewportSize != m_PrevFrameViewportSize)
    {
        CameraController::Get()->SetCameraAspectRatio(m_ViewportSize.x/m_ViewportSize.y);
    }
    m_PrevFrameViewportSize = m_ViewportSize;
    
    ImGui::GetWindowDrawList()->AddImage(   (void*)(intptr_t)Renderer::Get()->GetFramebuffer("RenderWorld")->GetRendererID(),
                                            ImVec2(ImGui::GetCursorScreenPos()),
                                            ImVec2(ImGui::GetCursorScreenPos().x + m_ViewportSize.x,ImGui::GetCursorScreenPos().y + m_ViewportSize.y), 
                                            ImVec2(0, 1), 
                                            ImVec2(1, 0));
    ImGui::End();
}

void WJohnnieViewportWidget::OnTick(double DeltaTime)
{}
