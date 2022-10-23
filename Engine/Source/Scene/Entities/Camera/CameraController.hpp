#pragma once

#include "Core/CoreMinimal.hpp"
#include "Events/IEventListener.hpp"
#include "Utilities/Singleton.hpp"

#include "Camera.hpp"
#include "Events/KeyEvent.hpp"
namespace Engine
{
	class ACamera;

	using namespace Events;

	class CameraController : public IEventListener, public Singleton<CameraController>
	{
	private:
		TSharedPtr<ACamera> m_ControlledCamera;
		bool m_bCursorCaputred = false;

		glm::vec2 m_PrevFrameMousePos;
		bool m_bSkipNextTickFrame = false;

	public:
		virtual void OnEvent(Event& Event) override;
		bool OnKeyPressed(KeyPressedEvent& KeyPressedEvent);
		bool OnKeyReleased(KeyReleasedEvent& KeyReleasedEvent);

		void OnTick(double DeltaTime);

		void SetViewTarget(TSharedPtr<ACamera> Camera);
		void SetCameraAspectRatio(float AspectRatio);

	public:
		TSharedPtr<ACamera> const& GetCamera(void) const
		{
			return m_ControlledCamera;
		}
		
		TSharedPtr<ACamera>& GetCamera(void)
		{
			return m_ControlledCamera;
		}
	};

	FORCEINLINE void CameraController::SetViewTarget(TSharedPtr<ACamera> Camera)
	{
		if(Camera)
		{
			m_ControlledCamera = MoveTemp(Camera);
			m_ControlledCamera->SetTickEnabled(true);
		}
	}

	FORCEINLINE void CameraController::SetCameraAspectRatio(float AspectRatio)
	{
		if (m_ControlledCamera)
		{
			m_ControlledCamera->SetAspectRatio(AspectRatio);
		}
	}

}