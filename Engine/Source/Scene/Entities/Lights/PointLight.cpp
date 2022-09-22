#include "EnginePCH.hpp"

#include "PointLight.hpp"
#include "Scene/SceneManager.hpp"
#include "Renderer/Renderer.hpp"
#include "../Meshes/BasicMeshGenerator.hpp"

namespace Engine
{
	APointLight::APointLight(glm::vec3 const&  WorldLocation, glm::vec3 const& LightColor)
		:	Actor(WorldLocation),
			m_LightColor(LightColor),
			m_SphereMesh(BasicMeshGenerator::CreateSphereMesh(0.1f,100,100))
	{}

    void APointLight::Draw(void) const
    {
        auto& shaderManager = Renderer::Get()->GetShaderManager();
        auto& lightMeshShader = shaderManager.GetResource("Shader_LightMesh");

        if(IsVisible())
        {
            const glm::mat4 modelMat = GetModelMat();

            if(m_bIsMeshVisible)
            {
                lightMeshShader->Bind();
                lightMeshShader->SetFloat3("uLightColor", m_LightColor);

                m_SphereMesh.Draw(lightMeshShader, modelMat);
            }
        }
    }

    void APointLight::OnTick(double DeltaTime)
    {}

    void APointLight::OnConstruct(void)
    {
        RegisterLight();
    }

    void APointLight::SetVisibility(bool Visible)
    {
        Actor::SetVisibility(Visible);
        
        if (Visible)
        {
            RegisterLight();
        }
        else
        {
            UnRegisterLight();
        }
    }

    void APointLight::RegisterLight(void)
    {
        auto& activeScene = SceneManager::Get()->GetActiveScene();
        CheckMsg(activeScene != nullptr, "No active scene.");

        if (!activeScene->GetLightingManager().IsLightRegistered(GetUUID()))
        {
            activeScene->GetLightingManager().RegisterPointLight(shared_from_this());
        }
    }

    void APointLight::UnRegisterLight(void)
    {
        auto& activeScene = SceneManager::Get()->GetActiveScene();
        CheckMsg(activeScene != nullptr, "No active scene.");

        if (activeScene->GetLightingManager().IsLightRegistered(GetUUID()))
        {
            activeScene->GetLightingManager().UnregisterPointLight(GetUUID());
        }
    }
}