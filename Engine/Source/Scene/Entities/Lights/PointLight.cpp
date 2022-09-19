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
			m_BoxMesh(BasicMeshGenerator::CreateBoxMesh(glm::vec3(0.1f,0.1f,0.1f)))
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

                m_BoxMesh.Draw(lightMeshShader, modelMat);
            }
        }
    }

    void APointLight::OnTick(double DeltaTime)
    {}

    void APointLight::OnConstruct(void)
    {
        auto& activeScene = SceneManager::Get()->GetActiveScene();
        CheckMsg(activeScene != nullptr, "No active scene.");

        activeScene->GetLightingManager().RegisterLight(shared_from_this());
    }

    void APointLight::SetVisibility(bool Visible)
    {
        Actor::SetVisibility(Visible);
        
        if (Visible)
        {
            
        }
        else
        {

        }
    }
    
}