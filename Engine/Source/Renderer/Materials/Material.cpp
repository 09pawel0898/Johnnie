#include "EnginePCH.hpp"

#include "Material.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"

namespace Engine
{
	void Material::SetBaseColor(glm::vec3 BaseColor)
	{
		m_MaterialUniform.BaseColor = BaseColor;
	}

	void Material::SetDiffuseTexture(std::shared_ptr<RHITexture2D> DiffuseTexture)
	{
		m_MaterialTextures.DiffuseTexture = std::move(DiffuseTexture);
		m_MaterialUniform.UseDiffuseMap = true;
	}

	void Material::SetSpecular(glm::vec3 Specular)
	{
		m_MaterialUniform.Specular = Specular;
	}

	void Material::SetShiness(float Shiness)
	{
		m_MaterialUniform.Shiness = Shiness;
	}

	void Material::SetMaterialEmissive(bool IsMaterialEmissive)
	{
		m_bIsMaterialEmissive = IsMaterialEmissive;
	}

	bool Material::IsMaterialEmissive(void) const
	{
		return m_bIsMaterialEmissive;
	}

	void Material::Bind(std::shared_ptr<RHIShader>& Shader) const
	{
		Shader->Bind();

		Shader->SetFloat3("uMaterial.BaseColor",m_MaterialUniform.BaseColor);

		/** Emissive materials use only base color for now.*/
		if (!IsMaterialEmissive())
		{
			Shader->SetFloat3("uMaterial.Specular", m_MaterialUniform.Specular);
			Shader->SetFloat("uMaterial.Shiness", m_MaterialUniform.Shiness);

			Shader->SetInt("uMaterial.UseDiffuseMap", (int32_t)m_MaterialUniform.UseDiffuseMap);

			if (m_MaterialUniform.UseDiffuseMap)
			{
				m_MaterialTextures.DiffuseTexture->Bind(0);
				std::string texName = GetUniformNameByTextureType(m_MaterialTextures.DiffuseTexture->GetType());
				Shader->SetInt(texName.c_str(), 0);
			}
		}
	}
}