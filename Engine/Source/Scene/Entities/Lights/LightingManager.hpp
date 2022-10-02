#pragma once

#include "Core/CoreMinimal.hpp"
#include "../Primitives/Object.hpp"
#include "glm/glm.hpp"

#include "PointLight.hpp"

namespace Engine
{
	class APointLight;

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

	public:
		void RegisterPointLight(std::shared_ptr<APointLight> const& PointLight);
		void UnregisterPointLight(OUUID const& LightID);
		std::optional<PointLightData> GetPointLightData(void) const;

	public:
		void RegisterLight(std::shared_ptr<APointLight> const& Light);
		bool IsLightRegistered(OUUID const& LightUUID);

		void CalculateLighting(void);
	};
}