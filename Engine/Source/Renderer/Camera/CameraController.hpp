#pragma once

#include "Core/CoreMinimal.hpp"
#include "Events/IEventListener.hpp"
#include "Utilities/Singleton.hpp"

#include "Camera.hpp"

namespace Engine
{
	class OCamera;

	using namespace Events;

	class CameraController : public IEventListener, Singleton<CameraController>
	{
	private:
		std::shared_ptr<OCamera> m_ControlledCamera;

	public:
		virtual void OnEvent(Event& Event) override;

	private:
		void SetCameraToControl(std::shared_ptr<OCamera> Camera);
	};

	FORCEINLINE void CameraController::SetCameraToControl(std::shared_ptr<OCamera> Camera)
	{
		m_ControlledCamera = std::move(Camera);
	}
}