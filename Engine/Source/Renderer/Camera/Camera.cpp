#include "EnginePCH.hpp"
#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/vector_angle.hpp>

namespace Engine
{
	OCamera::OCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation)
		:	m_FielfOfView(FieldOfView), 
			m_AspectRatio(AspectRatio), 
			m_NearClip(NearClip), 
			m_FarClip(FarClip)
	{
		SetLocation(SpawnLocation);

		UpdateProjection();
		UpdateView();
	}

	void OCamera::UpdateWorldCamera(void)
	{

		m_Target = { 0.f,-0.5f,0.f };

		m_Forward	= glm::normalize(m_Target - GetLocation());
		m_Right		= glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_Forward));
		m_Up		= glm::cross(m_Forward, m_Right);

		//std::cout << "m_Forward " << m_Forward.x << " " << m_Forward.y << " " << m_Forward.z<<"\n";
		//std::cout << "m_Right " << m_Right.x << " " << m_Right.y << " " << m_Right.z << "\n";
		//std::cout << "m_Forward " << m_Up.x << " " << m_Up.y << " " << m_Up.z << "\n";
	}

	void OCamera::UpdateProjection(void)
	{
		m_AspectRatio = 1280.f / 720.f;
		m_ProjectionMat = glm::perspective(glm::radians(m_FielfOfView), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void OCamera::UpdateView(void)
	{
		UpdateWorldCamera();

		m_ViewMat = glm::lookAt(GetLocation(), m_Target, m_Up);
	}
}