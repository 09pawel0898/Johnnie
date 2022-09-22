#include "EnginePCH.hpp"
#include "FloatingCamera.hpp"

#include "Core/Input/Input.hpp"
#include "GLFW/glfw3.h"

namespace Engine
{
	AFloatingCamera::AFloatingCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation)
		: ACamera(FieldOfView, AspectRatio, NearClip, FarClip, SpawnLocation)
	{
		SetRotation(glm::vec3(0.f, -15.f, 90.f));
	}

	void AFloatingCamera::OnMouseYawInput(float Rate)
	{
		static constexpr float yawSensitivity = 0.025f;

		Rate *= yawSensitivity;
		glm::vec3 const& currentRotation = GetRotation();

		SetRotation(glm::vec3(0.f, currentRotation.y, currentRotation.z + Rate));
	}

	void AFloatingCamera::OnMousePitchInput(float Rate)
	{
		static constexpr float pitchSensitivity = 0.025f;

		Rate *= pitchSensitivity;
		glm::vec3 currentRotation = GetRotation();

		float newPitch = currentRotation.y + Rate;

		if (newPitch > 89.0f)
		{
			newPitch = 89.0f;
		}
		if (newPitch < -89.0f)
		{
			newPitch = -89.0f;
		}
		SetRotation(glm::vec3(0.f, newPitch, currentRotation.z));
	}

	void AFloatingCamera::OnTick(double DeltaTime)
	{
		const float cameraSpeed = m_Speed * (float)DeltaTime;
		
		bool bIsMoving = false;

		auto IncreaseAccelerationMod = [this,&bIsMoving, DeltaTime]()
		{
			m_CammeraAccelerationMod = std::lerp(m_CammeraAccelerationMod, 0.7f, (float)(1 - pow(0.2, DeltaTime)));
			if (m_CammeraAccelerationMod > 0.7f)
			{
				m_CammeraAccelerationMod = 0.7f;
			}
			bIsMoving = true;
		};
		
		if(IsCameraPossessed())
		{
			if (Input::IsKeyPressed(KeyCode::W))
			{
				GetLocation() += cameraSpeed * m_Forward * m_CammeraAccelerationMod;
				IncreaseAccelerationMod();
			}
			if (Input::IsKeyPressed(KeyCode::S))
			{
				GetLocation() -= cameraSpeed * m_Forward * m_CammeraAccelerationMod;
				IncreaseAccelerationMod();
			}
			if (Input::IsKeyPressed(KeyCode::A))
			{
				GetLocation() -= glm::normalize(glm::cross(m_Forward, m_Up)) * cameraSpeed * m_CammeraAccelerationMod;
				IncreaseAccelerationMod();
			}
			if (Input::IsKeyPressed(KeyCode::D))
			{
				GetLocation() += glm::normalize(glm::cross(m_Forward, m_Up)) * cameraSpeed * m_CammeraAccelerationMod;
				IncreaseAccelerationMod();
			}
		}

		if (!bIsMoving)
		{
			m_CammeraAccelerationMod = std::lerp(m_CammeraAccelerationMod, 0.1f, (float)(1 - pow(0.1, DeltaTime)));
		}

		ACamera::OnTick(DeltaTime);
	}

	void AFloatingCamera::OnEvent(Event& Event)
	{
	}
}