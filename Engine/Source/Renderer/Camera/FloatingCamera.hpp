#pragma once

#include "Camera.hpp"

namespace Engine
{
	class OFloatingCamera final : public OCamera
	{
	public:
		int aaaa = 4;

	public:
		OFloatingCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation = { 0.f,0.0f,-3.f })
		:	OCamera(FieldOfView, AspectRatio, NearClip, FarClip, SpawnLocation)
		{}
	};
}