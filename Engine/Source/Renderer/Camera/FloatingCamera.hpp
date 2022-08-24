#pragma once

#include "Camera.hpp"

namespace Engine
{
	class OFloatingCamera final : public OCamera
	{
	public:
		OFloatingCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation = { 0.f,0.0f,-3.f });

	private:
		float m_Speed = 2.5f;
		float m_Acceleration = 10;

		float m_CammeraAccelerationMod = 0.1f;

	public:
		float GetAccelerationMod(void) const
		{
			return m_CammeraAccelerationMod;
		}
		
		void OnMouseYawInput(float Rate);
		void OnMousePitchInput(float Rate);

	public:
		virtual void OnTick(double DeltaTime) override;
		virtual void OnEvent(Event& Event) override;
	};
}