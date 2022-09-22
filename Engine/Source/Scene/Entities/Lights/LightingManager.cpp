#include "EnginePCH.hpp"

#include "LightingManager.hpp"
#include "PointLight.hpp"
#include "Renderer/Renderer.hpp"

namespace Engine
{
    void LightingManager::UnregisterPointLight(OUUID const& LightID)
    {
        auto registeredLight = std::find_if(m_PointLights.cbegin(), m_PointLights.cend(),
            [&LightID](std::pair<OUUID, std::weak_ptr<APointLight>> const& Element) -> bool
            {
                return LightID == Element.first;
            });
        if (registeredLight != m_PointLights.end())
        {
            m_PointLights.erase(registeredLight);
        }
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

    void LightingManager::RegisterLight(std::shared_ptr<APointLight> const& Light)
    {
        RegisterPointLight(Light);
    }

    bool LightingManager::IsLightRegistered(OUUID const& LightUUID)
    {
        return false;
    }
    
    void LightingManager::CalculateLighting(void)
    {
        auto pointLightData = GetPointLightData();
        if (pointLightData.has_value())
        {
            PointLightData& lightData = pointLightData.value();
            auto& staticMeshShader = Renderer::Get()->GetShaderManager().GetResource("Shader_StaticMesh");

            staticMeshShader->Bind();
            staticMeshShader->SetFloat3("uLightPosition", lightData.WorldLocation);
            staticMeshShader->SetFloat3("uLightColor", lightData.Color);
        }
    }
}