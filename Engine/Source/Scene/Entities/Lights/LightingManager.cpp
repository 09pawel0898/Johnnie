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

        if (pointLightData.has_value())
        {
            PointLightData& lightData = pointLightData.value();
            auto& staticMeshShader = Renderer::Get()->GetShaderManager().GetResource("Shader_StaticMesh");

            staticMeshShader->Bind();
            staticMeshShader->SetInt("uLight.bIsDirectional", 0);
            staticMeshShader->SetFloat3("uLight.Position", lightData.WorldLocation);

            glm::vec3 diffuseColor = lightData.Color * lightData.Diffuse;
            staticMeshShader->SetFloat3("uLight.Diffuse", diffuseColor);
            staticMeshShader->SetFloat3("uLight.Ambient", diffuseColor * lightData.Ambient);
            staticMeshShader->SetFloat3("uLight.Specular", glm::vec3(lightData.Specular));

            staticMeshShader->SetFloat("uLight.Constant", lightData.Constant);
            staticMeshShader->SetFloat("uLight.Linear", lightData.Linear);
            staticMeshShader->SetFloat("uLight.Quadratic", lightData.Quadratic);
            
            return;
        }

        auto directionalLightData = GetDirectionalLightData();
        if (directionalLightData.has_value())
        {
            DirectionalLightData& lightData = directionalLightData.value();
            auto& staticMeshShader = Renderer::Get()->GetShaderManager().GetResource("Shader_StaticMesh");

            staticMeshShader->Bind();
            staticMeshShader->SetInt("uLight.bIsDirectional", 1);
            staticMeshShader->SetFloat3("uLight.Direction", lightData.Direction);

            glm::vec3 diffuseColor = lightData.Color * lightData.Diffuse;
            staticMeshShader->SetFloat3("uLight.Diffuse", diffuseColor);
            staticMeshShader->SetFloat3("uLight.Ambient", diffuseColor * lightData.Ambient);
            staticMeshShader->SetFloat3("uLight.Specular", glm::vec3(lightData.Specular));

            return;
        }
    }
}