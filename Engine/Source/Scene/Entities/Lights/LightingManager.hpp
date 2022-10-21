#pragma once

#include "Core/CoreMinimal.hpp"
#include "../Primitives/Object.hpp"
#include "glm/glm.hpp"

#include "PointLight.hpp"
#include "DirectionalLight.hpp"

namespace Engine
{
	class LightingManager
	{
	private:
		std::vector<std::pair<OUUID,std::weak_ptr<APointLight>>> m_PointLights;
		std::vector<std::pair<OUUID,std::weak_ptr<ADirectionalLight>>> m_DirectionalLights;

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
		
		void RegisterDirectionalLight(std::shared_ptr<ADirectionalLight> const& DirectionalLight);
		void UnregisterDirectionalLight(OUUID const& LightID);
		std::optional<DirectionalLightData> GetDirectionalLightData(void) const;

		std::optional<glm::mat4> GetDirectionalLightDepthVP(void) const;

	private:
		template <typename TLightsContainer>
		void UnregisterLight_Internal(OUUID const& LightID, TLightsContainer& LightsContainer);
		
		template<typename TLightsContainer, typename TLightSPtr>
		void RegisterLight_Internal(TLightSPtr const& LightSPtr, TLightsContainer& LightsContainer);

	public:
		bool IsLightRegistered(OUUID const& LightUUID);
		void CalculateLighting(void);
	};

	template<typename TLightsContainer>
	FORCEINLINE void LightingManager::UnregisterLight_Internal(OUUID const& LightID, TLightsContainer& LightsContainer)
	{
		auto registeredLight = std::find_if(LightsContainer.cbegin(), LightsContainer.cend(),
			[&LightID](auto const& Element) -> bool
			{
				return LightID == Element.first;
			});
		if (registeredLight != LightsContainer.end())
		{
			LightsContainer.erase(registeredLight);
		}
	}

	template<typename TLightsContainer, typename TLightSPtr>
	FORCEINLINE void LightingManager::RegisterLight_Internal(TLightSPtr const& LightSPtr, TLightsContainer& LightsContainer)
	{
		if (LightsContainer.size() > 0)
		{
			CheckMsg(false, "Multiple lights not available yet.");
		}

		Check(LightsContainer.cend() == std::find_if(LightsContainer.cbegin(), LightsContainer.cend(),
			[&LightSPtr](auto const& Element) -> bool
			{
				return LightSPtr->GetUUID() == Element.first;
			}));

		LightsContainer.emplace_back(
			std::make_pair(LightSPtr->GetUUID(), std::weak_ptr<std::remove_reference_t<decltype(*std::declval<TLightSPtr>())>>(LightSPtr)));
	}
}