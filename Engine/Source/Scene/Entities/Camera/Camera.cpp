#include "EnginePCH.hpp"
#include "Camera.hpp"
#include "Core/Application/Application.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	ACamera::ACamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation)
		:	m_FielfOfView(FieldOfView),
			m_AspectRatio(AspectRatio),
			m_NearClip(NearClip),
			m_FarClip(FarClip)
	{
		SetLocation(SpawnLocation);
		SetRotation(glm::vec3(0.0f, 0.0f, 90.0f));

		m_AspectRatio = (float)(Core::Application::Get()->GetWindow()->GetWidth()) / (float)(Core::Application::Get()->GetWindow()->GetHeight());

		UpdateProjection();
		UpdateView();
	}

	void ACamera::UpdateWorldCamera(void)
	{
		auto const& CurrentRotation = GetRotation();

		m_Forward = glm::normalize(glm::vec3(
			cos(glm::radians(CurrentRotation.z)) * cos(glm::radians(CurrentRotation.y)),
			sin(glm::radians(CurrentRotation.y)),
			sin(glm::radians(CurrentRotation.z)) * cos(glm::radians(CurrentRotation.y))));

		m_Right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_Forward));
		m_Up = glm::cross(m_Forward, m_Right);
	}

	void ACamera::UpdateProjection(void)
	{
		m_ProjectionMat = glm::perspective(glm::radians(m_FielfOfView), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void ACamera::UpdateView(void)
	{
		UpdateWorldCamera();
		m_ViewMat = glm::lookAt(GetLocation(), GetLocation() + m_Forward, m_Up);
	}

	void ACamera::OnTick(double DeltaTime)
	{
		UpdateView();
	}
}