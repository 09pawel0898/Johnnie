#include "EnginePCH.hpp"

#include "Material.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/RHI/Platform/OpenGL/Resources/OpenGLTexture.hpp"
#include "Renderer/Renderer.hpp"
#include "Utilities/Platform/OpenGL/OpenGLHelpers.hpp"

namespace Engine
{
	void Material::SetBaseColor(glm::vec3 BaseColor)
	{
		m_Uniform.BaseColor = BaseColor;
		m_Uniform.UseDiffuseMap = false;
	}

	void Material::SetDiffuseTexture(TSharedPtr<RHITexture2D> DiffuseMapTexture)
	{
		m_Textures.DiffuseMapTexture = MoveTemp(DiffuseMapTexture);
		m_Uniform.UseDiffuseMap = true;
	}

	void Material::SetSpecularTexture(TSharedPtr<RHITexture2D> SpecularMapTexture)
	{
		m_Textures.SpecularMapTexture = MoveTemp(SpecularMapTexture);
		m_Uniform.UseSpecularMap = true;
	}

	void Material::SetNormalTexture(TSharedPtr<RHITexture2D> NormalMapTexture)
	{
		m_Textures.NormalMapTexture = MoveTemp(NormalMapTexture);
		m_Uniform.UseNormalMap = true;
	}

	bool Material::SetUseDiffuseTexture(bool Use)
	{
		if (m_Textures.DiffuseMapTexture != nullptr)
		{
			m_Uniform.UseDiffuseMap = Use;
			return true;
		}
		m_Uniform.UseDiffuseMap = false;
		return false;
	}

	bool Material::SetUseSpecularTexture(bool Use)
	{
		if (m_Textures.SpecularMapTexture != nullptr)
		{
			m_Uniform.UseSpecularMap = Use;
			return true;
		}
		m_Uniform.UseSpecularMap = false;
		return false;
	}

	bool Material::SetUseNormalTexture(bool Use)
	{
		if (m_Textures.NormalMapTexture != nullptr)
		{
			m_Uniform.UseNormalMap = Use;
			return true;
		}
		m_Uniform.UseNormalMap= false;
		return false;
	}

	void Material::SetSpecular(glm::vec3 SpecularMap)
	{
		m_Uniform.SpecularMap = SpecularMap;
		m_Uniform.UseSpecularMap = false;
	}

	void Material::SetShiness(float Shiness)
	{
		m_Uniform.Shiness = Shiness;
	}

	void Material::SetMaterialEmissive(bool IsMaterialEmissive)
	{
		m_Uniform.IsEmmisive = IsMaterialEmissive;
	}

	bool Material::IsMaterialEmissive(void) const
	{
		return m_Uniform.IsEmmisive;
	}

	void Material::SetTextureByType(RHIMapTextureType TextureType, TSharedPtr<RHITexture2D> Texture)
	{
		if (TextureType == RHIMapTextureType::DiffuseMap)
		{
			SetDiffuseTexture(Texture);
		}
		else if (TextureType == RHIMapTextureType::SpecularMap)
		{
			SetSpecularTexture(Texture);
		}
		else if (TextureType == RHIMapTextureType::NormalMap)
		{
			SetNormalTexture(Texture);
		}
	}

	void Material::Bind(TSharedPtr<RHIShader>& Shader) const
	{
		Shader->Bind();
		Shader->SetFloat3("uMaterial.BaseColor", m_Uniform.BaseColor);

		if (!IsMaterialEmissive())
		{
			// Setting material uniform parameters //
			Shader->SetFloat3("uMaterial.Specular", m_Uniform.SpecularMap);
			Shader->SetFloat("uMaterial.Shiness",	m_Uniform.Shiness);

			Shader->SetInt("uMaterial.UseDiffuseMap",	(int32_t)m_Uniform.UseDiffuseMap);
			Shader->SetInt("uMaterial.UseSpecularMap",	(int32_t)m_Uniform.UseSpecularMap);
			Shader->SetInt("uMaterial.UseNormalMap",	(int32_t)m_Uniform.UseNormalMap);

			// Binding Diffuse/Specular/Normal maps if selected //
			if (m_Uniform.UseDiffuseMap)
			{
				m_Textures.DiffuseMapTexture->Bind(0);
				std::string texName = GetUniformNameByTextureType(m_Textures.DiffuseMapTexture->GetType());
				Shader->SetInt(texName.c_str(), 0);
			}
			if (m_Uniform.UseSpecularMap)
			{
				m_Textures.SpecularMapTexture->Bind(1);
				std::string texName = GetUniformNameByTextureType(m_Textures.SpecularMapTexture->GetType());
				Shader->SetInt(texName.c_str(), 1);
			}
			if (m_Uniform.UseNormalMap)
			{
				m_Textures.NormalMapTexture->Bind(2);
				std::string texName = GetUniformNameByTextureType(m_Textures.NormalMapTexture->GetType());
				Shader->SetInt(texName.c_str(), 2);
			}
			
			// Bind shadow map //
			if (Renderer::Get()->GetApiType() == RenderingAPI::OpenGL)
			{
				Utility::OpenGL::BindTexture(Renderer::Get()->GetFramebuffer("ShadowMap")->GetDepthAttachmentID(), 3);
				Shader->SetInt("uTextureShadowMap", 3);
			}
		}
	}
}