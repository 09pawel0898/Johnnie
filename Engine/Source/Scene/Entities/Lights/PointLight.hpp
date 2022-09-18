#pragma once

#include "Core/CoreMinimal.hpp"
#include "../Primitives/Actor.hpp"
#include "../Meshes/Mesh.hpp"

#include "glm/glm.hpp"

namespace Engine
{
	class APointLight : public Actor
	{
	private:
		glm::vec3	m_LightColor;
		Mesh		m_BoxMesh;
		bool		m_bIsMeshVisible{ true };

	public:	
		APointLight(glm::vec3 const& WorldLocation = {0.f, 0.f, 0.f}, glm::vec3 const& LightColor = { 1.f, 1.f, 1.f });
		
	public:
		void SetBoxMeshVisibility(bool Visible);

		virtual void Draw(void) const override;
		void OnTick(double DeltaTime) override;

		/** Sets visibility and and registers/unregisters point light in lighting manager at the same time. */
		virtual void SetVisibility(bool Visible) override;

		/** Color */
		void SetColor(glm::vec3 LightColor);
		glm::vec3 GetColor(void) const;
	};

	FORCEINLINE void APointLight::SetBoxMeshVisibility(bool Visible)
	{
		m_bIsMeshVisible = Visible;
	}

	FORCEINLINE void APointLight::SetColor(glm::vec3 LightColor)
	{
		m_LightColor = LightColor;
	}
	
	FORCEINLINE glm::vec3 APointLight::GetColor(void) const
	{
		return m_LightColor;
	}

}