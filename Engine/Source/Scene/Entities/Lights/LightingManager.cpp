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

    void LightingManager::RegisterDirectionalLight(std::shared_ptr<ADirectionalLight> const& DirectionalLight)
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
        if (auto pointLight = m_DirectionalLights[0].second.lock())
        {
            return pointLight->GetData();
        }
        return std::nullopt;
    }
    
    void LightingManager::RegisterPointLight(std::shared_ptr<APointLight> const& PointLight)
    {
        RegisterLight_Internal(PointLight, m_PointLights);
    }

    bool LightingManager::IsLightRegistered(OUUID const& LightUUID)
    {
        return false;
    }
    
    void LightingManager::CalculateLighting(void)
    {
        auto pointLightData = GetPointLightData();

        auto& staticMeshShader = Renderer::Get()->GetShaderManager().GetResource("Shader_StaticMesh");

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