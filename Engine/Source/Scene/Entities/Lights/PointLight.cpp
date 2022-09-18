#include "EnginePCH.hpp"

#include "PointLight.hpp"
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
        static std::shared_ptr<RHIShader> pointLightBoxMeshShader = RHI::RHIShader::Create("PointLightShader", "Assets/Shaders/point_light.glsl");

        if(IsVisible())
        {
            const glm::mat4 modelMat = GetModelMat();

            if(m_bIsMeshVisible)
            {
                pointLightBoxMeshShader->Bind();
                pointLightBoxMeshShader->SetFloat3("uLightColor", m_LightColor);

                m_BoxMesh.Draw(pointLightBoxMeshShader, modelMat);
            }
        }
    }

    void APointLight::OnTick(double DeltaTime)
    {}

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