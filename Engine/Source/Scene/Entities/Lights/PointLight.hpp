#pragma once

#include "Core/CoreMinimal.hpp"
#include "../Primitives/Actor.hpp"
#include "../Meshes/Mesh.hpp"
#include "Renderer/Materials/Material.hpp"

#include "glm/glm.hpp"

namespace Engine
{
	struct PointLightData
	{
	public:
		glm::vec3 Color;
		glm::vec3 WorldLocation;

		float Ambient = 0.5f;
		float Diffuse = 1.0f;
		float Specular = 0.75f;
		
		float Constant = 1.f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;

		static std::unordered_map<int16_t,std::pair<float,float>> PointLightStrength;
	
	public:
		PointLightData() = default;
		explicit PointLightData(glm::vec3 const& Color, glm::vec3 const& WorldLocation = { 0.f,0.f,0.f })
			:	Color(Color),
				WorldLocation(WorldLocation)
		{}
	};

	class APointLight : public Actor, public SharedFromThis<APointLight>
	{
	private:
		PointLightData m_LightData;

		std::shared_ptr<Mesh>	m_SphereMesh;
		bool m_bIsMeshVisible{ true };

	public:	
		APointLight(glm::vec3 const& WorldLocation = {0.f, 0.f, 0.f}, glm::vec3 const& LightColor = { 1.f, 1.f, 1.f }, int8_t Strength = 4);
		
	public:
		void SetBoxMeshVisibility(bool Visible);

		virtual void Draw(void) const override;
		void OnTick(double DeltaTime) override;
		virtual void OnConstruct(void) override;

		/** Sets visibility and and registers/unregisters point light in lighting manager at the same time. */
		virtual void SetVisibility(bool Visible) override;

		void SetIntensity(int8_t Intensity /*0-10*/);

		/** Color */
		void SetColor(glm::vec3 LightColor);
		glm::vec3 GetColor(void) const;

		PointLightData const& GetData(void) const;
		PointLightData& GetData(void);

	private:
		std::shared_ptr<Material> m_SphereEmissiveMaterial;
		void InitializeMaterial(void);

		void RegisterLight(void);
		void UnRegisterLight(void);
	};

	FORCEINLINE void APointLight::SetBoxMeshVisibility(bool Visible)
	{
		m_bIsMeshVisible = Visible;
	}

	FORCEINLINE void APointLight::SetColor(glm::vec3 LightColor)
	{
		m_LightData.Color = LightColor;
		m_SphereEmissiveMaterial->SetBaseColor(LightColor);
	}
	
	FORCEINLINE glm::vec3 APointLight::GetColor(void) const
	{
		return m_LightData.Color;
	}

	FORCEINLINE PointLightData const& APointLight::GetData(void) const
	{
		return m_LightData;
	}

	FORCEINLINE PointLightData& APointLight::GetData(void)
	{
		return m_LightData;
	}
}