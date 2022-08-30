#pragma once

#include "Camera.hpp"

namespace Engine
{
	class OFloatingCamera final : public OCamera
	{
	public:
		OFloatingCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation = { 0.f,1.0f,-3.f });

	private:
		float m_Speed = 15.5f;
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

		void SetSpeed(float Speed);
		float GetSpeed(void) const;

		void SetAcceleration(float Acceleration);
		float GetAcceleration(void) const;
	};

	FORCEINLINE void OFloatingCamera::SetSpeed(float Speed)
	{
		m_Speed = Speed;
	}

	FORCEINLINE float OFloatingCamera::GetSpeed(void) const
	{
		return m_Speed;
	}
}