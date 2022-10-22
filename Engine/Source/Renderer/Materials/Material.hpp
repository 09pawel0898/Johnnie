#pragma once

#include "Core/CoreMinimal.hpp"
#include "glm/glm.hpp"
#include "../RHI/Resources/RHIResource.hpp"

namespace Engine
{
	namespace RHI
	{
		class RHITexture2D;
		class RHIShader;
	}
	using namespace RHI;

	struct MaterialUniform
	{
		glm::vec3 BaseColor{ 0.47f,0.47f,0.47f };
		bool UseDiffuseMap = false;

		glm::vec3 Specular{ 0.08f, 0.08f, 0.08f };
		bool UseSpecularMap = false;

		bool UseNormalMap = false;

		float Shiness = 8.f;

		MaterialUniform() = default;
		MaterialUniform(glm::vec3 const& BaseColor, glm::vec3 const& Specular, float Shiness)
			:	BaseColor(BaseColor), Specular(Specular), Shiness(Shiness)
		{}
	};

	struct MaterialTextures
	{
		std::shared_ptr<RHITexture2D> DiffuseTexture{ nullptr };
		std::shared_ptr<RHITexture2D> SpecularTexture{ nullptr };
		std::shared_ptr<RHITexture2D> NormalTexture{ nullptr };

		std::vector<std::reference_wrapper<std::shared_ptr<RHITexture2D>>> GetTextures(void)
		{
			return {	std::ref(DiffuseTexture),
						std::ref(SpecularTexture),
						std::ref(NormalTexture)};
		}
	};

	class Material : public RHIResource
	{
	private:
		MaterialUniform		m_MaterialUniform;
		MaterialTextures	m_MaterialTextures;

		bool				m_bIsMaterialEmissive{ false };

	public:
		Material() = default;
		~Material() = default;

		template <typename TUniform>
		requires std::is_base_of_v<MaterialUniform, TUniform>
			Material(TUniform&& Uniform)
			: m_MaterialUniform(std::forward<TUniform>(Uniform))
		{}

	public:
		void SetBaseColor(glm::vec3 BaseColor);

		void SetDiffuseTexture(std::shared_ptr<RHITexture2D> DiffuseTexture);
		void SetSpecularTexture(std::shared_ptr<RHITexture2D> SpecularTexture);
		void SetNormalTexture(std::shared_ptr<RHITexture2D> NormalTexture);
		
		bool SetUseDiffuseTexture(bool Use);
		bool SetUseSpecularTexture(bool Use);
		bool SetUseNormalTexture(bool Use);

		void SetSpecular(glm::vec3 Specular);

		void SetShiness(float Shiness);

		void SetMaterialEmissive(bool IsMaterialEmissive);
		bool IsMaterialEmissive(void) const;

		MaterialUniform const& GetMaterialUniform(void) const;
		MaterialTextures& GetMaterialTextures(void);

	public:
		void Bind(std::shared_ptr<RHIShader>& Shader) const;
	};

	FORCEINLINE MaterialUniform const& Material::GetMaterialUniform(void) const
	{
		return m_MaterialUniform;
	}

	FORCEINLINE MaterialTextures& Material::GetMaterialTextures(void)
	{
		return m_MaterialTextures;
	}

	namespace DefaultMaterials
	{
		extern const std::shared_ptr<Material> BasicWhite;
		extern const std::shared_ptr<Material> Emerald;
		extern const std::shared_ptr<Material> Gold;
		extern const std::shared_ptr<Material> Silver;
	}
}