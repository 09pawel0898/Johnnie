#pragma once

#include "Core/CoreMinimal.hpp"
#include "Events/IEventListener.hpp"
#include "Utilities/Singleton.hpp"

#include "Camera.hpp"

namespace Engine
{
	class OCamera;

	using namespace Events;

	class CameraController : public IEventListener, public Singleton<CameraController>
	{
	private:
		std::shared_ptr<OCamera> m_ControlledCamera;

	public:
		virtual void OnEvent(Event& Event) override;

		void OnTick(double DeltaTime);

		void SetViewTarget(std::shared_ptr<OCamera> Camera);

	public:
		std::shared_ptr<OCamera> const& GetCamera(void) const
		{
			return m_ControlledCamera;
		}
	};

	FORCEINLINE void CameraController::SetViewTarget(std::shared_ptr<OCamera> Camera)
	{
		m_ControlledCamera = std::move(Camera);
	}
}