#include "EnginePCH.hpp"
#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/vector_angle.hpp>

namespace Engine
{
	ACamera::ACamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation)
		: m_FielfOfView(FieldOfView),
		m_AspectRatio(AspectRatio),
		m_NearClip(NearClip),
		m_FarClip(FarClip)
	{
		SetLocation(SpawnLocation);
		SetRotation(glm::vec3(0.0f, 0.0f, 90.0f));

		UpdateProjection();
		UpdateView();
	}

	void ACamera::UpdateWorldCamera(void)
	{
		auto const& currentRotation = GetRotation();

		m_Forward = glm::normalize(glm::vec3(
			cos(glm::radians(currentRotation.z)) * cos(glm::radians(currentRotation.y)),
			sin(glm::radians(currentRotation.y)),
			sin(glm::radians(currentRotation.z)) * cos(glm::radians(currentRotation.y))));

		m_Right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_Forward));
		m_Up = glm::cross(m_Forward, m_Right);
	}

	void ACamera::UpdateProjection(void)
	{
		m_AspectRatio = 1280.0f / 720.0f;
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