#pragma once

#include "Core/CoreMinimal.hpp"

#include "Scene/Entities/Object.hpp"
#include "Scene/Entities/Tickable.hpp"
#include "Events/IEventListener.hpp"

namespace Engine
{
	using namespace Events;

	class OCamera : public Object, public Tickable, public IEventListener
	{
	protected:
		/** View Parameters */
		glm::mat4 m_ViewMat = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMat = glm::mat4(1.0f);
		
		glm::vec3 m_Forward;
		glm::vec3 m_Up;
		glm::vec3 m_Right;

		/** Frustrum Parameters*/
		float m_FielfOfView;
		float m_AspectRatio;
		float m_NearClip;
		float m_FarClip;

		bool m_bIsCameraPossessed = false;

	public:
		OCamera() = delete;
		OCamera(OCamera const&) = delete;

		virtual ~OCamera() = default;
		
		OCamera(float FieldOfView, float AspectRatio, float NearClip, float FarClip, glm::vec3 SpawnLocation = { 0.f,0.0f,-3.f });

		glm::mat4 const& GetProjectionMat(void) const;
		glm::mat4 const& GetViewMat(void) const;
		glm::mat4 const& GetViewProjectionMat(void) const;

		glm::vec3 const& GetForwardVec(void) const;
		glm::vec3 const& GetUpVec(void) const;
		glm::vec3 const& GetRightVec(void) const;

		bool IsCameraPossessed(void) const;
		void SetCameraPossessed(bool Possessed);

		virtual void OnTick(double DeltaTime) override;
		virtual void OnEvent(Event& Event) override {}

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

	FORCEINLINE glm::mat4 const& OCamera::GetProjectionMat(void) const
	{
		return m_ProjectionMat;
	}

	FORCEINLINE glm::mat4 const& OCamera::GetViewMat(void) const
	{
		return m_ViewMat;
	}

	FORCEINLINE glm::mat4 const& OCamera::GetViewProjectionMat(void) const
	{
		return m_ProjectionMat * m_ViewMat;
	}
			    
	FORCEINLINE glm::vec3 const& OCamera::GetForwardVec(void) const
	{
		return m_Forward;
	}
	
	FORCEINLINE glm::vec3 const& OCamera::GetUpVec(void) const
	{
		return m_Up;
	}

	FORCEINLINE glm::vec3 const& OCamera::GetRightVec(void) const
	{
		return m_Right;
	}

	FORCEINLINE bool OCamera::IsCameraPossessed(void) const
	{
		return m_bIsCameraPossessed;
	}

	FORCEINLINE void OCamera::SetCameraPossessed(bool Possessed)
	{
		m_bIsCameraPossessed = Possessed;
	}

}