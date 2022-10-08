#pragma once

#include "Core/CoreMinimal.hpp"
#include "../Primitives/Actor.hpp"
#include "../Meshes/Mesh.hpp"
#include "Renderer/Materials/Material.hpp"

#include "glm/glm.hpp"

namespace Engine
{
	struct DirectionalLightData
	{
		glm::vec3 Color;
		glm::vec3 Direction;

		float Ambient = 0.5f;
		float Diffuse = 1.0f;
		float Specular = 0.75f;

		DirectionalLightData() = default;

		explicit DirectionalLightData(glm::vec3 const& Color, glm::vec3 const& Direction = { -0.2f,-1.0f,-0.3f })
			:	Color(Color),
				Direction(Direction)
		{}
	};

	class ADirectionalLight : public Actor, public SharedFromThis<ADirectionalLight>
	{
	private:
		DirectionalLightData m_LightData;

	public:	
		ADirectionalLight(glm::vec3 const& Direction = { -0.2f,-1.0f,-0.3f }, glm::vec3 const& LightColor = { 1.f, 1.f, 1.f });
		
	public:
		void OnTick(double DeltaTime) override;
		virtual void OnConstruct(void) override;

		/** Color */
		void SetColor(glm::vec3 LightColor);
		glm::vec3 const& GetColor(void) const;

		void SetDirection(glm::vec3 Direction);
		glm::vec3 const& GetDirection(void);

		DirectionalLightData const& GetData(void) const;
		DirectionalLightData& GetData(void);

		void SetVisibility(bool Visible);

	private:
		void RegisterLight(void);
		void UnRegisterLight(void);
	};

	FORCEINLINE void ADirectionalLight::SetColor(glm::vec3 LightColor)
	{
		m_LightData.Color = LightColor;
	}
	
	FORCEINLINE glm::vec3 const& ADirectionalLight::GetColor(void) const
	{
		return m_LightData.Color;
	}

	FORCEINLINE void ADirectionalLight::SetDirection(glm::vec3 Direction)
	{
		m_LightData.Direction = Direction;
	}

	FORCEINLINE glm::vec3 const& ADirectionalLight::GetDirection(void)
	{
		return m_LightData.Direction;
	}

	FORCEINLINE DirectionalLightData const& ADirectionalLight::GetData(void) const
	{
		return m_LightData;
	}

	FORCEINLINE DirectionalLightData& ADirectionalLight::GetData(void)
	{
		return m_LightData;
	}
}