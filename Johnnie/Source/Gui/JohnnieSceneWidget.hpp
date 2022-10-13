#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Gui.hpp>

#include "ImGui/ImGuiWidgetBase.hpp"

class MaterialSlotWidget final
{
private:
	std::shared_ptr<Material>& m_MaterialRef;
	bool m_bUseDiffuseMap{ false };
	bool m_bUseSpecularMap{ false };
	bool m_bUseNormalMap{ false };

	float m_Specular = 0.5f;
	int32_t m_Shininess = 3;

	ImVec4 m_BaseColor{ ImVec4() };

	ImGui::FileBrowser m_FileBrowser{};
	RHITextureType m_TextureTypeFileBrowserOpenedFor = RHITextureType::BasicTexture2D;

public:
	MaterialSlotWidget(std::shared_ptr<Material>& MaterialRef);

	std::shared_ptr<Material>& GetMaterialRef(void)
	{
		return m_MaterialRef;
	}

	std::shared_ptr<Material> const& GetMaterialRef(void) const
	{
		return m_MaterialRef;
	}

	void RefreshMaterial(void);
	void OnRenderGui(void);

private:
	void InitializeFileBrowser(void);
};

class WJohnnieSceneWidget final : public ImGuiWidgetBase
{
private:
	/** Point Light */
	std::shared_ptr<APointLight>	m_ManagedPointLight{ nullptr };
	ImColor							m_PointLightColor;
	
	/** Static/Skeletal Mesh and Materials */
	std::optional<std::string> m_MeshSubtabName = std::nullopt;
	std::vector<MaterialSlotWidget> m_MaterialSlotWidgets;


	/** Rendering */
	bool m_bWireframeMode{ false };

public:
	WJohnnieSceneWidget();

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;

	void SetManagedPointLight(std::shared_ptr<APointLight> PointLight);

private:
	void OnRenderLightingSubtab();
	void OnRenderRenderingSubtab();
	void OnRenderMeshSubtab();

	void PrepareStaticMeshSubtab(AStaticMesh* StaticMesh);
	void ClearStaticMeshSubtabContent(void);

	void SetRendererWireframemode(bool Enabled);
};