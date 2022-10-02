#include "EnginePCH.hpp"

#include "PointLight.hpp"
#include "Scene/SceneManager.hpp"
#include "Renderer/Renderer.hpp"
#include "../Meshes/BasicMeshGenerator.hpp"

namespace Engine
{
	APointLight::APointLight(glm::vec3 const&  WorldLocation, glm::vec3 const& LightColor)
		:	Actor(WorldLocation),
			m_LightData(PointLightData(LightColor,WorldLocation)),
			m_SphereMesh(BasicMeshGenerator::CreateSphereMesh(0.1f,100,100))
	{
        InitializeMaterial();
    }

    void APointLight::Draw(void) const
    {
        if(IsVisible())
        {
            const glm::mat4 modelMat = GetModelMat();

            if(m_bIsMeshVisible)
            {
                m_SphereMesh->Draw(modelMat);
            }
        }
    }

    void APointLight::OnTick(double DeltaTime)
    {
        m_LightData.WorldLocation = GetLocation();
    }

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

    void APointLight::InitializeMaterial(void)
    {
        MaterialUniform sphereMatUniform;
        sphereMatUniform.BaseColor = m_LightData.Color;

        m_SphereEmissiveMaterial = std::make_shared<Material>(std::move(sphereMatUniform));
        m_SphereEmissiveMaterial->SetMaterialEmissive(true);

        m_SphereMesh->SetHardMaterialReference(m_SphereEmissiveMaterial);
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