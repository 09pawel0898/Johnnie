#pragma once

#include "Core/CoreMinimal.hpp"
#include "../Primitives/Object.hpp"
#include "glm/glm.hpp"

namespace Engine
{
	class APointLight;

	struct PointLightData
	{
		glm::vec3 Color;
		glm::vec3 WorldLocation;

		PointLightData() = default;
		explicit PointLightData(glm::vec3 const& Color, glm::vec3 const& WorldLocation = { 0.f,0.f,0.f })
			:	Color(Color),
				WorldLocation(WorldLocation)
		{}
	};

	class LightingManager
	{
	private:
		std::vector<std::pair<OUUID,std::weak_ptr<APointLight>>> m_PointLights;

	public:
		LightingManager() = default;

		LightingManager(LightingManager const&) = delete;
		LightingManager& operator=(LightingManager const&) = delete;

		LightingManager(LightingManager&&) = delete;
		LightingManager& operator=(LightingManager&&) = delete;

	private:
		void RegisterPointLight(std::shared_ptr<APointLight> const& PointLight);
		void UnregisterPointLight(OUUID const& LightID);

	public:
		std::optional<PointLightData> GetPointLightData(void) const;

	public:
		void RegisterLight(std::shared_ptr<APointLight> const& Light);
		bool IsLightRegistered(OUUID const& LightUUID);
	};
}