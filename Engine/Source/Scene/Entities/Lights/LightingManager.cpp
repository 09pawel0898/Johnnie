#include "EnginePCH.hpp"

#include "LightingManager.hpp"
#include "PointLight.hpp"

namespace Engine
{
    void LightingManager::UnregisterPointLight(OUUID const& LightID)
    {
    }

    std::optional<PointLightData> LightingManager::GetPointLightData(void) const
    {
        if (m_PointLights.size() != 1)
        {
            return std::nullopt;
        }
        if(auto pointLight = m_PointLights[0].second.lock())
        {
            return PointLightData{ pointLight->GetColor(),pointLight->GetLocation() };
        }
        return std::nullopt;
    }
    
    void LightingManager::RegisterPointLight(std::shared_ptr<APointLight> const& PointLight)
    {
        if (m_PointLights.size() > 0)
        {
            CheckMsg(false, "Multiple lights not available yet.");
        }

        Check(  m_PointLights.cend() ==
                std::find_if(m_PointLights.cbegin(), m_PointLights.cend(),
                [&PointLight](std::pair<OUUID, std::weak_ptr<APointLight>> const& Element) -> bool
                {
                    return PointLight->GetUUID() == Element.first;
                }));

        m_PointLights.emplace_back(
            std::make_pair(PointLight->GetUUID(), std::weak_ptr<APointLight>(PointLight)));
    }

    bool LightingManager::IsLightRegistered(OUUID const& LightUUID)
    {
        return false;
    }
}