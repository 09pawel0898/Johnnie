#include "JohnnieViewportWidget.hpp"

#include <Engine/Gui.hpp>
#include <Engine/Renderer.hpp>

WJohnnieViewportWidget::WJohnnieViewportWidget()
{
	SetTickEnabled(false);
}

void WJohnnieViewportWidget::OnRenderGui(void)
{
    ImGui::Begin("Viewport");
    //get the mouse position
    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::GetWindowDrawList()->AddImage(
        (void*)(intptr_t)Renderer::Get()->GetFramebuffer("RenderWorld")->GetRendererID(),
        ImVec2(ImGui::GetCursorScreenPos()),
        ImVec2(ImGui::GetCursorScreenPos().x + Application::Get()->GetWindow()->GetWidth() / 2,
            ImGui::GetCursorScreenPos().y + Application::Get()->GetWindow()->GetHeight() / 2), ImVec2(0, 1), ImVec2(1, 0));

    //we are done working with this window
    ImGui::End();
}

void WJohnnieViewportWidget::OnTick(double DeltaTime)
{}
