#pragma once

#include "Camera.hpp"

namespace Engine
{
	class OFloatingCamera final : public OCamera
	{
	public:
		OFloatingCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation = { 0.f,0.0f,-3.f });

	public:
		virtual void OnTick(double DeltaTime) override;
		
		virtual void OnEvent(Event& Event) override;
	};
}