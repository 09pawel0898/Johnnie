#include "JohnnieSceneWidget.hpp"
#include "../JohnnieDelegates.hpp"

#include <Engine/Renderer.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Debug.hpp>

#include <imgui_internal.h>
#include <glm/gtx/rotate_vector.hpp>

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
	if (auto pointLight = m_ManagedPointLight.lock())
	{
		ImVec4 color = m_PointLightColor.Value;
		pointLight->SetColor(glm::vec3(color.x, color.y, color.z));
	}
	if (auto directionalLight = m_ManagedDirectionalLight.lock())
	{
		ImVec4 color = m_DirectionalLightColor.Value;
		directionalLight->SetColor(glm::vec3(color.x, color.y, color.z));
	}

	for (auto& materialSlotWidget : m_MaterialSlotWidgets)
	{
		materialSlotWidget.RefreshMaterial();
	}
}

void WJohnnieSceneWidget::SetManagedPointLight(std::weak_ptr<APointLight> PointLight)
{
	m_ManagedPointLight = std::move(PointLight);

	if(auto pointLight = m_ManagedPointLight.lock())
	{
		m_PointLightColor =
		{
			pointLight->GetColor().r,
			pointLight->GetColor().g,
			pointLight->GetColor().b
		};

		m_bPointLightVisible = false;
		m_bPointLightMeshVisible = false;
	}
}

void WJohnnieSceneWidget::SetManagedDirectionalLight(std::weak_ptr<ADirectionalLight> DirectionalLight)
{
	m_ManagedDirectionalLight = std::move(DirectionalLight);

	if (auto directionalLight = m_ManagedDirectionalLight.lock())
	{
		m_DirectionalLightColor =
		{
			directionalLight->GetColor().r,
			directionalLight->GetColor().g,
			directionalLight->GetColor().b
		};
	}
}

void WJohnnieSceneWidget::OnRenderLightingSubtab()
{
	if (ImGui::CollapsingHeader("Lighting"))
	{
		if (ImGui::TreeNode("Directional Light"))
		{
			if (auto directionalLight = m_ManagedDirectionalLight.lock())
			{
				auto& lightData = directionalLight->GetData();

				ImGui::Dummy(ImVec2(0, 5));
				ImGui::Separator();

				ImGui::Dummy(ImVec2(0, 2));
				if (ImGui::SliderFloat("Light Angle", &m_DirectionalLightAngle, 0.f, 360.f))
				{
					static glm::vec3 v = { -0.2f,-1.0f,-0.3f };
					directionalLight->SetDirection(glm::rotateY(v, glm::radians(m_DirectionalLightAngle)));
				}
				ImGui::Dummy(ImVec2(0, 2));

				ImGui::ColorPicker4("", (float*)&m_DirectionalLightColor,
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

				ImGui::Image((void*)(intptr_t)Renderer::Get()->GetFramebuffer("ShadowMap")->GetDepthAttachmentID(), ImVec2(256, 256));
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Point Light"))
		{
			if (auto pointLight = m_ManagedPointLight.lock())
			{
				auto& lightData = pointLight->GetData();

				ImGui::Dummy(ImVec2(0, 5));
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0, 2));

				if (ImGui::Checkbox("Enabled", &m_bPointLightVisible))
				{
					pointLight->SetVisibility(m_bPointLightVisible);
				}

				if (ImGui::Checkbox("Mesh Visible", &m_bPointLightMeshVisible))
				{
					pointLight->SetBoxMeshVisibility(m_bPointLightMeshVisible);
				}
				ImGui::Dummy(ImVec2(0, 2));
				ImGui::ColorPicker4("", (float*)&m_PointLightColor,
					ImGuiColorEditFlags_NoDragDrop |
					ImGuiColorEditFlags_NoSidePreview |
					ImGuiColorEditFlags_NoSmallPreview);
				
				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::SliderFloat("Diffuse", &lightData.Diffuse, 0.0f, 1.0f);
				
				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::SliderFloat("Specular", &lightData.Specular, 0.0f, 1.0f);
				
				ImGui::Separator();
			}
			ImGui::TreePop();
		}
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
			if (m_MaterialSlotWidgets.size() > 0)
			{
				std::string materialsHeader = "Materials [" + std::to_string(m_MaterialSlotWidgets.size()) + "]";

				ImGui::Separator();
				ImGui::Text(materialsHeader.c_str());
				ImGui::Separator();

				for (int16_t idx = 0; idx < m_MaterialSlotWidgets.size(); idx++)
				{
					std::string slotName = "Slot " + std::to_string(idx);
					if (ImGui::TreeNode(slotName.c_str()))
					{
						m_MaterialSlotWidgets[idx].OnRenderGui();
						ImGui::TreePop();
					}
				}
			}
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

		m_bUseDiffuseMap =		uniform.UseDiffuseMap;
		m_bUseSpecularMap =		uniform.UseSpecularMap;
		m_bUseNormalMap =		uniform.UseNormalMap;

		m_BaseColor = ImVec4(	uniform.BaseColor.x, 
								uniform.BaseColor.y, 
								uniform.BaseColor.z, 1.0f);
		
		m_Specular = (uniform.Specular.r + uniform.Specular.g + uniform.Specular.b) / 3.f;
		m_Shininess = (int32_t)std::log2(uniform.Shiness);
	}
}

void MaterialSlotWidget::OnRenderGui(void)
{
	// Base Color //
	{
		ImGui::Dummy(ImVec2(0.f, 5.f));

		if (ImGui::ColorEdit3("Base Color", (float*)&m_BaseColor))
		{
			m_MaterialRef->SetBaseColor(glm::vec3(m_BaseColor.x, m_BaseColor.y, m_BaseColor.z));
			m_MaterialRef->SetUseDiffuseTexture(m_bUseDiffuseMap);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));
	}

	MaterialTextures& materialTextures = m_MaterialRef->GetMaterialTextures();
	
	// Diffuse Map //
	{
		if (ImGui::BeginTable("DiffuseMapTable", 2, ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV))
		{
			ImGui::TableNextRow();
			for (uint16_t columnIdx = 0; columnIdx < 2; ++columnIdx)
			{
				ImGui::TableSetColumnIndex(columnIdx);

				if (columnIdx == 0)
				{
					if (materialTextures.DiffuseTexture != nullptr)
					{
						ImGui::Image((void*)(intptr_t)materialTextures.DiffuseTexture->GetRendererID(), ImVec2(80, 80));
					}
					else
					{
						auto noTextureImg = Renderer::Get()->GetTexture2DManager().GetResource("Assets/Textures/NoTexture_Image.png");
						ImGui::Image((void*)(intptr_t)noTextureImg->GetRendererID(), ImVec2(80, 80));
					}
				}
				else if (columnIdx == 1)
				{
					ImGui::Dummy(ImVec2(0.f, 2.f));
					if (ImGui::Button("Load Diffuse Map", ImVec2(120, 20)))
					{
						m_FileBrowser.Open();
						m_TextureTypeFileBrowserOpenedFor = RHITextureType::Diffuse;
					}
					ImGui::Dummy(ImVec2(0.f, 23.f));

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
				}
			}
			ImGui::EndTable();
		}
	}

	// Specular //
	ImGui::Dummy(ImVec2(0.f, 5.f));
	if (ImGui::SliderFloat("Specular", &m_Specular, 0.f, 1.f))
	{
		m_MaterialRef->SetSpecular(glm::vec3(m_Specular, m_Specular, m_Specular));
		m_MaterialRef->SetUseSpecularTexture(m_bUseSpecularMap);
	}
	ImGui::Dummy(ImVec2(0.f, 5.f));

	// Specular Map //
	{
		if (ImGui::BeginTable("SpecularMapTable", 2, ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV))
		{
			ImGui::TableNextRow();
			for (uint16_t columnIdx = 0; columnIdx < 2; ++columnIdx)
			{
				ImGui::TableSetColumnIndex(columnIdx);

				if (columnIdx == 0)
				{
					if (materialTextures.SpecularTexture != nullptr)
					{
						ImGui::Image((void*)(intptr_t)materialTextures.SpecularTexture->GetRendererID(), ImVec2(80, 80));
					}
					else
					{
						auto noTextureImg = Renderer::Get()->GetTexture2DManager().GetResource("Assets/Textures/NoTexture_Image.png");
						ImGui::Image((void*)(intptr_t)noTextureImg->GetRendererID(), ImVec2(80, 80));
					}
				}
				else if (columnIdx == 1)
				{
					ImGui::Dummy(ImVec2(0.f, 2.f));
					if (ImGui::Button("Load Specular Map", ImVec2(133, 20)))
					{
						m_FileBrowser.Open();
						m_TextureTypeFileBrowserOpenedFor = RHITextureType::Specular;
					}
					ImGui::Dummy(ImVec2(0.f, 23.f));

					if (ImGui::Checkbox("Use Specular Map", &m_bUseSpecularMap))
					{
						if (m_bUseSpecularMap)
						{
							if (!m_MaterialRef->SetUseSpecularTexture(true))
							{
								m_bUseSpecularMap = false;
							}
						}
						else
						{
							m_MaterialRef->SetSpecular(glm::vec3(m_Specular, m_Specular, m_Specular));
						}
					}
				}
			}
			ImGui::EndTable();
		}
	}

	// Shininess //
	ImGui::Dummy(ImVec2(0.f, 5.f));
	if (ImGui::SliderInt("Shininess", &m_Shininess,1,8))
	{
		m_MaterialRef->SetShiness((float)pow(2.0, (double)m_Shininess));
	}
	ImGui::Dummy(ImVec2(0.f, 5.f));

	// Normal Map //
	{
		if (ImGui::BeginTable("NormalMapTable", 2, ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV))
		{
			ImGui::TableNextRow();
			for (uint16_t columnIdx = 0; columnIdx < 2; ++columnIdx)
			{
				ImGui::TableSetColumnIndex(columnIdx);

				if (columnIdx == 0)
				{
					if (materialTextures.NormalTexture != nullptr)
					{
						ImGui::Image((void*)(intptr_t)materialTextures.NormalTexture->GetRendererID(), ImVec2(80, 80));
					}
					else
					{
						auto noTextureImg = Renderer::Get()->GetTexture2DManager().GetResource("Assets/Textures/NoTexture_Image.png");
						ImGui::Image((void*)(intptr_t)noTextureImg->GetRendererID(), ImVec2(80, 80));
					}
				}
				else if (columnIdx == 1)
				{
					ImGui::Dummy(ImVec2(0.f, 2.f));
					if (ImGui::Button("Load Normal Map", ImVec2(133, 20)))
					{
						m_FileBrowser.Open();
						m_TextureTypeFileBrowserOpenedFor = RHITextureType::Normal;
					}
					ImGui::Dummy(ImVec2(0.f, 23.f));

					if (ImGui::Checkbox("Use Normal Map", &m_bUseNormalMap))
					{
						if (!m_MaterialRef->SetUseNormalTexture(m_bUseNormalMap))
						{
							m_bUseNormalMap = false;
						}
					}
				}
			}
			ImGui::EndTable();
		}
	}

	// File Browser Displaying //
	m_FileBrowser.Display();
	
	std::string selectedFileName;
	if (m_FileBrowser.HasSelected())
	{
		selectedFileName = m_FileBrowser.GetSelected().string();

		Renderer::Get()->GetTexture2DManager().LoadResource(selectedFileName);
		
		auto loadedTexture = Renderer::Get()->GetTexture2DManager().GetResource(selectedFileName);

		if (m_TextureTypeFileBrowserOpenedFor == RHITextureType::Diffuse)
		{
			loadedTexture->SetType(RHITextureType::Diffuse);
			m_MaterialRef->SetDiffuseTexture(std::move(loadedTexture));
		}
		else if (m_TextureTypeFileBrowserOpenedFor == RHITextureType::Specular)
		{
			loadedTexture->SetType(RHITextureType::Specular);
			m_MaterialRef->SetSpecularTexture(std::move(loadedTexture));
		}
		else if (m_TextureTypeFileBrowserOpenedFor == RHITextureType::Normal)
		{
			loadedTexture->SetType(RHITextureType::Normal);
			m_MaterialRef->SetNormalTexture(std::move(loadedTexture));
		}

		m_FileBrowser.ClearSelected();
	}
}

void MaterialSlotWidget::InitializeFileBrowser(void)
{
	m_FileBrowser.SetTitle("Select Texture");
	m_FileBrowser.SetTypeFilters({ ".png",".jpg"});
}
