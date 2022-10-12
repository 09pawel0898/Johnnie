#include "JohnnieCameraWidget.hpp"

#include <Engine/Gui.hpp>
#include <Engine/Camera.hpp>

WJohnnieCameraWidget::WJohnnieCameraWidget()
{
	SetTickInterval(0.05f);
}

void WJohnnieCameraWidget::OnRenderGui(void)
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

    ImGui::Begin("Camera");
    {
        ImGui::TextUnformatted("Camera Transform");

        if (ImGui::BeginTable("TransformTable", 4, ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV))
        {
            for (uint16_t rowIdx = 0; rowIdx < 5; ++rowIdx)
            {
                ImGui::TableNextRow();          
                for (uint16_t columnIdx = 0; columnIdx < 4; ++columnIdx)
                {
                    ImGui::TableSetColumnIndex(columnIdx);

                    if (rowIdx == 1)
                    {
                        if (columnIdx == 1) ImGui::TextUnformatted("X");
                        if (columnIdx == 2) ImGui::TextUnformatted("Y");
                        if (columnIdx == 3) ImGui::TextUnformatted("Z");
                    }

                    if (rowIdx == 2)
                    {
                        if (columnIdx == 0) ImGui::TextUnformatted("Location");
                        if (columnIdx == 1) ImGui::Text("%f", m_CameraData.Location.x);
                        if (columnIdx == 2) ImGui::Text("%f", m_CameraData.Location.y);
                        if (columnIdx == 3) ImGui::Text("%f", m_CameraData.Location.z);
                        ImGui::Dummy(ImVec2(0, 5));
                    }

                    if (rowIdx == 3)
                    {
                        if (columnIdx == 1) ImGui::TextUnformatted("Roll");
                        if (columnIdx == 2) ImGui::TextUnformatted("Pitch");
                        if (columnIdx == 3) ImGui::TextUnformatted("Yaw");
                    }

                    if (rowIdx == 4)
                    {
                        if (columnIdx == 0) ImGui::TextUnformatted("Rotation");
                        if (columnIdx == 1) ImGui::Text("%f", m_CameraData.Rotation.x);
                        if (columnIdx == 2) ImGui::Text("%f", m_CameraData.Rotation.y);
                        if (columnIdx == 3) ImGui::Text("%f", m_CameraData.Rotation.z);
                    }
                }
            
            }
            ImGui::EndTable();
        }
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Text("Camera Parameters");

        ImGui::SliderFloat("Camera Speed", &m_CameraMutableData.Speed, 5.0f, 80.0f);
        
    }
    ImGui::End();
}

void WJohnnieCameraWidget::OnTick(double DeltaTime)
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
