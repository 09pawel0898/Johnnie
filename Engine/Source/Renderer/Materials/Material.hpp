#pragma once

#include "Core/CoreMinimal.hpp"
#include "glm/glm.hpp"
#include "../RHI/Resources/RHIResource.hpp"

#include <vector>

namespace Engine
{
	namespace RHI
	{
		class RHITexture2D;
		class RHIShader;
	}
	using namespace RHI;

	enum MaterialsEvaluateMethod : uint8_t
	{
		FromModelImporter,
		OnePerSubmesh
	};

	struct MaterialUniform
	{
		glm::vec3 BaseColor{ 0.47f,0.47f,0.47f };
		bool UseDiffuseMap{ false };

		glm::vec3 Specular{ 0.08f, 0.08f, 0.08f };
		bool UseSpecularMap{ false };

		bool UseNormalMap{ false };

		bool IsEmmisive{ false };

		float Shiness = 8.f;

		MaterialUniform() = default;

		MaterialUniform(glm::vec3 const& BaseColor, glm::vec3 const& Specular, float Shiness)
			:	BaseColor(BaseColor), Specular(Specular), Shiness(Shiness)
		{}
	};

	struct MaterialTextures
	{
		TSharedPtr<RHITexture2D> DiffuseMapTexture	{ nullptr };
		TSharedPtr<RHITexture2D> SpecularMapTexture	{ nullptr };
		TSharedPtr<RHITexture2D> NormalMapTexture	{ nullptr };
	};

	class Material final : public RHIResource
	{
	private:
		MaterialUniform		m_Uniform;
		MaterialTextures	m_Textures;

	public:
		Material() = default;
		~Material() = default;

		explicit Material(MaterialUniform const& Uniform)
			:	m_Uniform(Uniform)
		{}

	public:
		void SetBaseColor(glm::vec3 BaseColor);
		void SetDiffuseTexture(TSharedPtr<RHITexture2D> DiffuseMapTexture);
		bool SetUseDiffuseTexture(bool Use);

		void SetSpecular(glm::vec3 Specular);
		void SetSpecularTexture(TSharedPtr<RHITexture2D> SpecularTexture);
		bool SetUseSpecularTexture(bool Use);

		void SetNormalTexture(TSharedPtr<RHITexture2D> NormalTexture);		
		bool SetUseNormalTexture(bool Use);

		void SetShiness(float Shiness);

		void SetMaterialEmissive(bool IsMaterialEmissive);
		bool IsMaterialEmissive(void) const;

	public:
		MaterialUniform const& GetMaterialUniform(void) const;
		MaterialTextures& GetMaterialTextures(void);

		void Bind(TSharedPtr<RHIShader>& Shader) const;
	};

	FORCEINLINE MaterialUniform const& Material::GetMaterialUniform(void) const
	{
		return m_Uniform;
	}

	FORCEINLINE MaterialTextures& Material::GetMaterialTextures(void)
	{
		return m_Textures;
	}

	namespace DefaultMaterials
	{
		extern const TSharedPtr<Material> BasicWhite;
		extern const TSharedPtr<Material> Emerald;
		extern const TSharedPtr<Material> Gold;
		extern const TSharedPtr<Material> Silver;
	}
}