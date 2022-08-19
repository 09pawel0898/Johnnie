#pragma once

#include "Core/CoreMinimal.hpp"

#include <glm/glm.hpp>

namespace Engine
{
	class Camera
	{
	protected:
		/** View Parameters */
		glm::mat4 m_ViewMat = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMat = glm::mat4(1.0f);
		
		/** Camera World Parameters */
		glm::vec3 m_Location;
		glm::vec3 m_Rotation;

		glm::vec3 m_Forward;
		glm::vec3 m_Up;
		glm::vec3 m_Right;

		glm::vec3 m_Target;

		/** Frustrum Parameters*/
		float m_FielfOfView;
		float m_AspectRatio;
		float m_NearClip;
		float m_FarClip;

	public:
		Camera() = delete;
		Camera(Camera const&) = delete;

		virtual ~Camera() = default;
		
		Camera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation = { 0.f,-0.5f,-3.f });

		glm::mat4 const& GetProjectionMat(void) const;
		glm::mat4 const& GetViewMat(void) const;
		glm::mat4 const& GetViewProjectionMat(void) const;

		glm::vec3 const& GetLocation(void) const;
		glm::vec3 const& GetRotation(void) const;

		glm::vec3 const& GetForwardVec(void) const;
		glm::vec3 const& GetUpVec(void) const;
		glm::vec3 const& GetRightVec(void) const;


	private:
		void UpdateWorldCamera(void);
	
	public:
		/** 
		 *	Updates projection matrix. Perform it after every Viewport size change.
		 */
		void UpdateProjection(void);

		/**
		 *	Updates view matrix. Perform it every frame.
		 */
		void UpdateView(void);
	};

	FORCEINLINE glm::mat4 const& Camera::GetProjectionMat(void) const
	{
		return m_ProjectionMat;
	}

	FORCEINLINE glm::mat4 const& Camera::GetViewMat(void) const
	{
		return m_ViewMat;
	}

	FORCEINLINE glm::mat4 const& Camera::GetViewProjectionMat(void) const
	{
		return m_ProjectionMat * m_ViewMat;
	}

	FORCEINLINE glm::vec3 const& Camera::GetLocation(void) const
	{
		return m_Location;
	}
	
	FORCEINLINE glm::vec3 const& Camera::GetRotation(void) const
	{
		return m_Rotation;
	}
			    
	FORCEINLINE glm::vec3 const& Camera::GetForwardVec(void) const
	{
		return m_Forward;
	}
	
	FORCEINLINE glm::vec3 const& Camera::GetUpVec(void) const
	{
		return m_Up;
	}

	FORCEINLINE glm::vec3 const& Camera::GetRightVec(void) const
	{
		return m_Right;
	}
}