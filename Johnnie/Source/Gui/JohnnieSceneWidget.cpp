#include "JohnnieSceneWidget.hpp"
#include "../JohnnieDelegates.hpp"

#include <Engine/Renderer.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Debug.hpp>

WJohnnieSceneWidget::WJohnnieSceneWidget()
{
	Renderer::Get()->GetTexture2DManager().LoadResource("Assets/Textures/NoTexture_Image.png");
	
	SceneDelegates::Get()->OnStaticMeshLoaded.AddLambda([this](AStaticMesh* StaticMesh)
	{
		PrepareStaticMeshSubtab(StaticMesh);
	});

	JohnnieDelegates::Get()->OnBeginLoadingNewModel.AddLambda([this](std::string const& FilePath) 
	{
		ClearStaticMeshSubtabContent();
	});
}

void WJohnnieSceneWidget::OnRenderGui(void)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Scene");
	{
		OnRenderRenderingSubtab();
		OnRenderLightingSubtab();
		OnRenderMeshSubtab();
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

	for (auto& materialSlotWidget : m_MaterialSlotWidgets)
	{
		materialSlotWidget.RefreshMaterial();
	}
}

void WJohnnieSceneWidget::SetManagedPointLight(std::shared_ptr<APointLight> PointLight)
{
	if(PointLight)
	{
		m_ManagedPointLight = std::move(PointLight);

		m_PointLightColor =
		{
			m_ManagedPointLight->GetColor().r,
			m_ManagedPointLight->GetColor().g,
			m_ManagedPointLight->GetColor().b
		};
	}
}

void WJohnnieSceneWidget::OnRenderLightingSubtab()
{
	if (ImGui::CollapsingHeader("Lighting"))
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, 20);
		ImGui::NextColumn();

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
				ImGui::SliderFloat("Ambient", &lightData.Ambient, 0.0f, 1.0f);
				
				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::SliderFloat("Diffuse", &lightData.Diffuse, 0.0f, 1.0f);
				
				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::SliderFloat("Specular", &lightData.Specular, 0.0f, 1.0f);
				
				ImGui::Separator();
			}
		}
		ImGui::Columns(1);
	}
}

void WJohnnieSceneWidget::OnRenderRenderingSubtab()
{
	if (ImGui::CollapsingHeader("Rendering"))
	{
		if (ImGui::Checkbox("Wireframe Mode", &m_bWireframeMode))
		{
			SetRendererWireframemode(m_bWireframeMode);
		}
	}
}

void WJohnnieSceneWidget::OnRenderMeshSubtab()
{
	if (m_MeshSubtabName.has_value())
	{
		if (ImGui::CollapsingHeader(m_MeshSubtabName.value().c_str()))
		{
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnWidth(0, 20);
			ImGui::NextColumn();

			if (m_MaterialSlotWidgets.size() > 0)
			{
				std::string materialsHeader = "Materials [" + std::to_string(m_MaterialSlotWidgets.size()) + "]";

				ImGui::Separator();
				ImGui::Text(materialsHeader.c_str());
				ImGui::Separator();

				for (int16_t idx = 0; idx < m_MaterialSlotWidgets.size(); idx++)
				{
					std::string slotName = "Slot " + std::to_string(idx);
					if (ImGui::CollapsingHeader(slotName.c_str()))
					{
						m_MaterialSlotWidgets[idx].OnRenderGui();
					}
				}
			}
			ImGui::Columns(1);
		}
	}
}

void WJohnnieSceneWidget::PrepareStaticMeshSubtab(AStaticMesh* StaticMesh)
{
	m_MeshSubtabName = "Static Mesh";

	m_MaterialSlotWidgets.clear();

	for (int8_t idx = 0; idx < StaticMesh->GetNumMaterials(); ++idx)
	{
		auto material = StaticMesh->GetMaterialInSlot(idx);
		if (material.has_value())
		{
			m_MaterialSlotWidgets.emplace_back(MaterialSlotWidget(material->get()));
		}
	}
}

void WJohnnieSceneWidget::SetRendererWireframemode(bool Enabled)
{
	Renderer::Get()->SetWireframeMode(Enabled);
}

void WJohnnieSceneWidget::ClearStaticMeshSubtabContent(void)
{
	m_MaterialSlotWidgets.clear();
}

MaterialSlotWidget::MaterialSlotWidget(std::shared_ptr<Material>& MaterialRef)
	: m_MaterialRef(MaterialRef)
{
	InitializeFileBrowser();
}

void MaterialSlotWidget::RefreshMaterial(void)
{
	if (m_MaterialRef != nullptr)
	{
		MaterialUniform const& uniform = m_MaterialRef->GetMaterialUniform();

		m_bUseDiffuseMap = uniform.UseDiffuseMap;
		m_bUseSpecularMap = uniform.UseSpecularMap;
		m_BaseColor = ImVec4(uniform.BaseColor.x, uniform.BaseColor.y, uniform.BaseColor.z, 1.0f);
	}
}

void MaterialSlotWidget::OnRenderGui(void)
{
	{
		ImGui::Dummy(ImVec2(0.f, 5.f));

		if (ImGui::ColorEdit3("Base Color", (float*)&m_BaseColor))
		{
			m_MaterialRef->SetBaseColor(glm::vec3(m_BaseColor.x, m_BaseColor.y, m_BaseColor.z));
			m_MaterialRef->SetUseDiffuseTexture(m_bUseDiffuseMap);
		}

		ImGui::Dummy(ImVec2(0.f, 10.f));
	}

	MaterialTextures& materialTextures = m_MaterialRef->GetMaterialTextures();

	if (materialTextures.DiffuseTexture != nullptr)
	{
		ImGui::Image((void*)(intptr_t)materialTextures.DiffuseTexture->GetRendererID(), ImVec2(80, 80));
	}
	else
	{
		auto noTextureImg = Renderer::Get()->GetTexture2DManager().GetResource("Assets/Textures/NoTexture_Image.png");
		ImGui::Image((void*)(intptr_t)noTextureImg->GetRendererID(), ImVec2(80, 80));
	}

	
	if (ImGui::Button("...", ImVec2(60, 20)))
	{
		m_FileBrowser.Open();
	}
	m_FileBrowser.Display();

	if (ImGui::Checkbox("Use Diffuse Map", &m_bUseDiffuseMap))
	{
		if (m_bUseDiffuseMap)
		{
			if (!m_MaterialRef->SetUseDiffuseTexture(true))
			{
				m_bUseDiffuseMap = false;
			}
		}
		else
		{
			m_MaterialRef->SetBaseColor(glm::vec3(m_BaseColor.x, m_BaseColor.y, m_BaseColor.z));
		}
	}
	//
	//if (ImGui::Checkbox("Use Specular Map", &m_bUseSpecularMap))
	//{
	//	LOG(Core, Trace, "b");
	//}
}

void MaterialSlotWidget::InitializeFileBrowser(void)
{
	m_FileBrowser.SetTitle("Select Texture");
	m_FileBrowser.SetTypeFilters({ ".png",".jpg" });
}
