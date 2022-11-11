#include "EnginePCH.hpp"

#include "LightingManager.hpp"
#include "PointLight.hpp"
#include "Renderer/Renderer.hpp"

namespace Engine
{
    void LightingManager::UnregisterPointLight(OUUID const& LightID)
    {
        UnregisterLight_Internal(LightID, m_PointLights);
    }

    std::optional<PointLightData> LightingManager::GetPointLightData(void) const
    {
        if (m_PointLights.size() != 1)
        {
            return std::nullopt;
        }
        if(auto pointLight = m_PointLights[0].second.lock())
        {
            return pointLight->GetData();
        }
        return std::nullopt;
    }

    void LightingManager::RegisterDirectionalLight(TSharedPtr<ADirectionalLight> const& DirectionalLight)
    {
        RegisterLight_Internal(DirectionalLight, m_DirectionalLights);
    }

    void LightingManager::UnregisterDirectionalLight(OUUID const& LightID)
    {
        UnregisterLight_Internal(LightID, m_DirectionalLights);
    }

    std::optional<DirectionalLightData> LightingManager::GetDirectionalLightData(void) const
    {
        if (m_DirectionalLights.size() != 1)
        {
            return std::nullopt;
        }
        if (auto directionalLight = m_DirectionalLights[0].second.lock())
        {
            return directionalLight->GetData();
        }
        return std::nullopt;
    }

    std::optional<glm::mat4> LightingManager::GetDirectionalLightDepthVP(void) const
    {
        if (m_DirectionalLights.size() != 1)
        {
            return std::nullopt;
        }
        if (auto directionalLight = m_DirectionalLights[0].second.lock())
        {
            return directionalLight->GetDepthVP();
        }
        return std::nullopt;
    }
    
    void LightingManager::RegisterPointLight(TSharedPtr<APointLight> const& PointLight)
    {
        RegisterLight_Internal(PointLight, m_PointLights);
    }

    bool LightingManager::IsLightRegistered(OUUID const& LightUUID)
    {
        return 
            (m_PointLights.cend() != std::find_if(m_PointLights.cbegin(), m_PointLights.cend(),
            [&LightUUID](auto const& Element) -> bool
            {
                return LightUUID == Element.first;
            }) 
                || 
            (m_DirectionalLights.cend() != std::find_if(m_DirectionalLights.cbegin(), m_DirectionalLights.cend(),
            [&LightUUID](auto const& Element) -> bool
            {
                return LightUUID == Element.first;
            })));
    }
    
    void LightingManager::CalculateLighting(void)
    {
        auto pointLightData = GetPointLightData();

        auto& staticMeshShader = Renderer::Get()->GetShaderManager().GetResource("Shader_Mesh");

        if (pointLightData.has_value())
        {
            PointLightData& lightData = pointLightData.value();

            staticMeshShader->Bind();

            staticMeshShader->SetInt("uNumPointLights", 1);
            staticMeshShader->SetFloat3("uPointLights[0].Position", lightData.WorldLocation);

            glm::vec3 diffuseColor = lightData.Color * lightData.Diffuse;
            staticMeshShader->SetFloat3("uPointLights[0].Diffuse", diffuseColor);
            staticMeshShader->SetFloat3("uPointLights[0].Ambient", diffuseColor * lightData.Ambient);
            staticMeshShader->SetFloat3("uPointLights[0].Specular", glm::vec3(lightData.Specular));

            staticMeshShader->SetFloat("uPointLights[0].Constant", lightData.Constant);
            staticMeshShader->SetFloat("uPointLights[0].Linear", lightData.Linear);
            staticMeshShader->SetFloat("uPointLights[0].Quadratic", lightData.Quadratic);
        }
        else
        {
            staticMeshShader->Bind();
            staticMeshShader->SetInt("uNumPointLights", 0);
        }

        auto directionalLightData = GetDirectionalLightData();
        if (directionalLightData.has_value())
        {
            DirectionalLightData& lightData = directionalLightData.value();

            staticMeshShader->Bind();
            staticMeshShader->SetFloat3("uDirectionalLight.Direction", lightData.Direction);

            glm::vec3 diffuseColor = lightData.Color * lightData.Diffuse;
            staticMeshShader->SetFloat3("uDirectionalLight.Diffuse", diffuseColor);
            staticMeshShader->SetFloat3("uDirectionalLight.Ambient", diffuseColor * lightData.Ambient);
            staticMeshShader->SetFloat3("uDirectionalLight.Specular", glm::vec3(lightData.Specular));
        }
    }
}