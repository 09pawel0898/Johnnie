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
        auto& MeshShader = Renderer::Get()->GetShaderManager().GetResource("Shader_Mesh");

        if (auto Data = GetPointLightData(); Data.has_value())
        {
            PointLightData& LightData = Data.value();

            MeshShader->Bind();
             
            MeshShader->SetInt("uNumPointLights", 1);
            MeshShader->SetFloat3("uPointLights[0].Position", LightData.WorldLocation);

            glm::vec3 DiffuseColor = LightData.Color * LightData.DiffuseMap;
            MeshShader->SetFloat3("uPointLights[0].Diffuse", DiffuseColor);
            MeshShader->SetFloat3("uPointLights[0].Ambient", DiffuseColor * LightData.Ambient);
            MeshShader->SetFloat3("uPointLights[0].Specular", glm::vec3(LightData.SpecularMap));

            MeshShader->SetFloat("uPointLights[0].Constant", LightData.Constant);
            MeshShader->SetFloat("uPointLights[0].Linear", LightData.Linear);
            MeshShader->SetFloat("uPointLights[0].Quadratic", LightData.Quadratic);
        }
        else
        {
            MeshShader->Bind();
            MeshShader->SetInt("uNumPointLights", 0);
        }

        if (auto Data = GetDirectionalLightData(); Data.has_value())
        {
            DirectionalLightData& LightData = Data.value();

            MeshShader->Bind();
            MeshShader->SetFloat3("uDirectionalLight.Direction", LightData.Direction);

            glm::vec3 DiffuseColor = LightData.Color * LightData.Diffuse;
            MeshShader->SetFloat3("uDirectionalLight.Diffuse", DiffuseColor);
            MeshShader->SetFloat3("uDirectionalLight.Ambient", DiffuseColor * LightData.Ambient);
            MeshShader->SetFloat3("uDirectionalLight.Specular", glm::vec3(LightData.Specular));
        }
    }
}