#pragma once

#include "Core/CoreMinimal.hpp"
#include "Events/IEventListener.hpp"
#include "Utilities/Singleton.hpp"

#include "Camera.hpp"
#include "Events/KeyEvent.hpp"
namespace Engine
{
	class OCamera;

	using namespace Events;

	class CameraController : public IEventListener, public Singleton<CameraController>
	{
	private:
		std::shared_ptr<OCamera> m_ControlledCamera;
		bool m_bCursorCaputred = false;

		glm::vec2 m_PrevFrameMousePos;
		bool m_bSkipNextTickFrame = false;

	public:
		virtual void OnEvent(Event& Event) override;
		bool OnKeyPressed(KeyPressedEvent& KeyPressedEvent);
		bool OnKeyReleased(KeyReleasedEvent& KeyReleasedEvent);

		void OnTick(double DeltaTime);

		void SetViewTarget(std::shared_ptr<OCamera> Camera);

	public:
		std::shared_ptr<OCamera> const& GetCamera(void) const
		{
			return m_ControlledCamera;
		}
		
		std::shared_ptr<OCamera>& GetCamera(void)
		{
			return m_ControlledCamera;
		}
	};

	FORCEINLINE void CameraController::SetViewTarget(std::shared_ptr<OCamera> Camera)
	{
		m_ControlledCamera = std::move(Camera);
		m_ControlledCamera->SetTickEnabled(true);
	}
}