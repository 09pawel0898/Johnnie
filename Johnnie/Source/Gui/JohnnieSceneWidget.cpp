#include "JohnnieSceneWidget.hpp"

#include <Engine/Renderer.hpp>

WJohnnieSceneWidget::WJohnnieSceneWidget()
{
}

void WJohnnieSceneWidget::OnRenderGui(void)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Scene");
	{
		if (ImGui::CollapsingHeader("Rendering"))
		{
			if (ImGui::Checkbox("Wireframe Mode", &m_bWireframeMode))
			{
				SetRendererWireframemode(m_bWireframeMode);
			}
		}
		if (ImGui::CollapsingHeader("Point Light"))
		{
			if (m_ManagedPointLight)
			{
				auto& lightData = m_ManagedPointLight->GetData();

				ImGui::Separator();
				ImGui::Text("Light Parmeters");

				ImGui::Separator();
				ImGui::ColorPicker4("", (float*)&m_PointLightColor, 
					ImGuiColorEditFlags_NoDragDrop | 
					ImGuiColorEditFlags_NoSidePreview | 
					ImGuiColorEditFlags_NoSmallPreview);

				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::SliderFloat("Ambient",	&lightData.Ambient, 0.0f, 1.0f);

				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::SliderFloat("Diffuse",	&lightData.Diffuse, 0.0f, 1.0f);

				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::SliderFloat("Specular",	&lightData.Specular, 0.0f, 1.0f);

				ImGui::Separator();
			}
		}
	}
	ImGui::End();
}

void WJohnnieSceneWidget::OnTick(double DeltaTime)
{
	if (m_ManagedPointLight)
	{
		ImVec4 color = m_PointLightColor.Value;
		m_ManagedPointLight->SetColor(glm::vec3(color.x, color.y, color.z));
	}
}

void WJohnnieSceneWidget::SetManagedPointLight(std::shared_ptr<APointLight> PointLight)
{
	m_ManagedPointLight = std::move(PointLight);

	m_PointLightColor =
	{
		m_ManagedPointLight->GetColor().r,
		m_ManagedPointLight->GetColor().g,
		m_ManagedPointLight->GetColor().b
	};
}

void WJohnnieSceneWidget::SetRendererWireframemode(bool Enabled)
{
	Renderer::Get()->SetWireframeMode(Enabled);
}
