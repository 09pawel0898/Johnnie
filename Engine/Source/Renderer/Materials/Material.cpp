#include "EnginePCH.hpp"

#include "Material.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/RHI/Platform/OpenGL/Resources/OpenGLTexture.hpp"
#include "Renderer/Renderer.hpp"

namespace Engine
{
	void Material::SetBaseColor(glm::vec3 BaseColor)
	{
		m_MaterialUniform.BaseColor = BaseColor;
		m_MaterialUniform.UseDiffuseMap = false;
	}

	void Material::SetDiffuseTexture(TSharedPtr<RHITexture2D> DiffuseTexture)
	{
		m_MaterialTextures.DiffuseTexture = MoveTemp(DiffuseTexture);
		m_MaterialUniform.UseDiffuseMap = true;
	}

	void Material::SetSpecularTexture(TSharedPtr<RHITexture2D> SpecularTexture)
	{
		m_MaterialTextures.SpecularTexture = MoveTemp(SpecularTexture);
		m_MaterialUniform.UseSpecularMap = true;
	}

	void Material::SetNormalTexture(TSharedPtr<RHITexture2D> NormalTexture)
	{
		m_MaterialTextures.NormalTexture = MoveTemp(NormalTexture);
		m_MaterialUniform.UseNormalMap = true;
	}

	bool Material::SetUseDiffuseTexture(bool Use)
	{
		if (m_MaterialTextures.DiffuseTexture != nullptr)
		{
			m_MaterialUniform.UseDiffuseMap = Use;
			return true;
		}
		m_MaterialUniform.UseDiffuseMap = false;
		return false;
	}

	bool Material::SetUseSpecularTexture(bool Use)
	{
		if (m_MaterialTextures.SpecularTexture != nullptr)
		{
			m_MaterialUniform.UseSpecularMap = Use;
			return true;
		}
		m_MaterialUniform.UseSpecularMap = false;
		return false;
	}

	bool Material::SetUseNormalTexture(bool Use)
	{
		if (m_MaterialTextures.NormalTexture != nullptr)
		{
			m_MaterialUniform.UseNormalMap = Use;
			return true;
		}
		m_MaterialUniform.UseNormalMap= false;
		return false;
	}

	void Material::SetSpecular(glm::vec3 Specular)
	{
		m_MaterialUniform.Specular = Specular;
		m_MaterialUniform.UseSpecularMap = false;
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

	void Material::Bind(TSharedPtr<RHIShader>& Shader) const
	{
		Shader->Bind();

		Shader->SetFloat3("uMaterial.BaseColor",m_MaterialUniform.BaseColor);

		/** Emissive materials use only base color for now.*/
		if (!IsMaterialEmissive())
		{
			Shader->SetFloat3("uMaterial.Specular", m_MaterialUniform.Specular);
			Shader->SetFloat("uMaterial.Shiness", m_MaterialUniform.Shiness);

			Shader->SetInt("uMaterial.UseDiffuseMap", (int32_t)m_MaterialUniform.UseDiffuseMap);
			Shader->SetInt("uMaterial.UseSpecularMap", (int32_t)m_MaterialUniform.UseSpecularMap);
			Shader->SetInt("uMaterial.UseNormalMap", (int32_t)m_MaterialUniform.UseNormalMap);

			if (m_MaterialUniform.UseDiffuseMap)
			{
				m_MaterialTextures.DiffuseTexture->Bind(0);
				std::string texName = GetUniformNameByTextureType(m_MaterialTextures.DiffuseTexture->GetType());
				Shader->SetInt(texName.c_str(), 0);
			}
			if (m_MaterialUniform.UseSpecularMap)
			{
				m_MaterialTextures.SpecularTexture->Bind(1);
				std::string texName = GetUniformNameByTextureType(m_MaterialTextures.SpecularTexture->GetType());
				Shader->SetInt(texName.c_str(), 1);
			}
			if (m_MaterialUniform.UseNormalMap)
			{
				m_MaterialTextures.NormalTexture->Bind(2);
				std::string texName = GetUniformNameByTextureType(m_MaterialTextures.NormalTexture->GetType());
				Shader->SetInt(texName.c_str(), 2);
			}
			
			OpenGLTexture2D::BindOpenGLTexture(Renderer::Get()->GetFramebuffer("ShadowMap")->GetDepthAttachmentID(), 3);
			//Renderer::Get()->GetTexture2DManager().GetResource("Assets/Textures/DepthTexture.png")->Bind(3);
			Shader->SetInt("uTextureShadowMap", 3);
		}
	}
}