#include "EnginePCH.hpp"

#include "PointLight.hpp"
#include "Scene/SceneManager.hpp"
#include "Renderer/Renderer.hpp"
#include "../Meshes/BasicMeshGenerator.hpp"

namespace Engine
{
    std::unordered_map<int16_t, std::pair<float, float>> PointLightData::PointLightStrength =
    {
                            // Distances

        {0,{0.7f,1.8f}},        // 7
        {1,{0.35f,0.44f}},      // 13
        {2,{0.22f,0.20f}},      // 20
        {3,{0.14f,0.07f}},      // 32
        {4,{0.09f,0.032f}},     // 50
        {5,{0.07f,0.017f}},     // 65
        {6,{0.045f,0.0075f}},   // 100
        {7,{0.027f,0.0028f}},   // 160
        {8,{0.022f,0.0019f}},   // 200
        {9,{0.014f,0.0007f}},   // 325
        {10,{0.007f,0.0002f}},  // 600
    };

	APointLight::APointLight(glm::vec3 const&  WorldLocation, glm::vec3 const& LightColor, int8_t Strength)
		:	Actor(WorldLocation),
			m_LightData(PointLightData(LightColor,WorldLocation)),
			m_SphereMesh(BasicMeshGenerator::CreateSphereMesh(0.1f,100,100))
	{
        InitializeMaterial();
        SetIntensity(Strength);
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

    void APointLight::SetIntensity(int8_t Intensity)
    {
        if (PointLightData::PointLightStrength.find(Intensity) == PointLightData::PointLightStrength.end())
        {
            LOG(Core, Warning, "Can't set light intensity of value {0}, acceptable range [0-10]", Intensity);
            return;
        };
        auto strength = PointLightData::PointLightStrength[Intensity];
        m_LightData.Linear = strength.first;
        m_LightData.Quadratic = strength.second;
    }

    void APointLight::InitializeMaterial(void)
    {
        MaterialUniform sphereMatUniform;
        sphereMatUniform.BaseColor = m_LightData.Color;

        m_SphereEmissiveMaterial = MakeShared<Material>(MoveTemp(sphereMatUniform));
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