#include "JohnnieCameraDataWidget.hpp"

#include <Engine/Gui.hpp>
#include <Engine/Camera.hpp>

WJohnnieCameraDataWidget::WJohnnieCameraDataWidget()
{
	SetTickInterval(0.05f);
}

void WJohnnieCameraDataWidget::OnRenderGui(void)
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

    ImGui::Begin("Camera");
    {
        ImGui::Separator();
        ImGui::Text("Transform");
        ImGui::Separator();

        ImGui::Text("              X        Y        Z");
        ImGui::Text("Location    : %f %f %f", m_CameraData.Location.x, m_CameraData.Location.y, m_CameraData.Location.z);
        ImGui::Text("              Roll     Pitch    Yaw");
        ImGui::Text("Rotation    : %f %f %f", m_CameraData.Rotation.x, m_CameraData.Rotation.y, m_CameraData.Rotation.z);

        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::Text("AccelerationMod    : %f", m_CameraData.CurrentAccelerationMod);
        
        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        ImGui::Separator();

        ImGui::Text("Adjust");
        ImGui::SliderFloat("Camera Speed", &m_CameraMutableData.Speed, 5.0f, 80.0f);


        ImGui::Separator();
    }
    ImGui::End();
}

void WJohnnieCameraDataWidget::OnTick(double DeltaTime)
{
    if (auto floatingCamera = Cast<AFloatingCamera>(CameraController::Get()->GetCamera()))
    {
        if (!m_bDefaultValuesInitialized)
        {
            m_CameraMutableData.Speed = floatingCamera->GetSpeed();
            m_bDefaultValuesInitialized = true;
        }

        m_CameraData.Location = floatingCamera->GetLocation();
        m_CameraData.Rotation = floatingCamera->GetRotation();
        m_CameraData.CurrentAccelerationMod = floatingCamera->GetAccelerationMod();

        floatingCamera->SetSpeed(m_CameraMutableData.Speed);
    }
}
