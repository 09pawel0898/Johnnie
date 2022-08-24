#include "CameraDataWidget.hpp"

#include <Engine/Gui.hpp>
#include <Engine/Camera.hpp>

CameraDataWidget::CameraDataWidget()
{
	SetTickInterval(0.05f);
}

void CameraDataWidget::OnRenderGui(void)
{
    ImGui::Begin("Camera");
    {
        ImGui::Separator();
        
        ImGui::Text("              X        Y        Z");
        ImGui::Text("Location    : %f %f %f", m_CameraData.Location.x, m_CameraData.Location.y, m_CameraData.Location.z);
        ImGui::Text("              Roll     Pitch    Yaw");
        ImGui::Text("Rotation    : %f %f %f", m_CameraData.Rotation.x, m_CameraData.Rotation.y, m_CameraData.Rotation.z);

        ImGui::Separator();
        ImGui::Text("AccelerationMod    : %f", m_CameraData.CurrentAccelerationMod);

        ImGui::Separator();
    }
    ImGui::End();
}
#include <iostream>
void CameraDataWidget::OnTick(double DeltaTime)
{
    if (auto floatingCamera = Cast<OFloatingCamera>(CameraController::Get()->GetCamera()))
    {
        m_CameraData.Location = floatingCamera->GetLocation();
        m_CameraData.Rotation = floatingCamera->GetRotation();
        m_CameraData.CurrentAccelerationMod = floatingCamera->GetAccelerationMod();
    }
}
