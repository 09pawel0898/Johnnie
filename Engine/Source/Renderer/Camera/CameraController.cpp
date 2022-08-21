#include "EnginePCH.hpp"

#include "CameraController.hpp"
#include "Renderer/Camera/Camera.hpp"

#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"

namespace Engine
{
	void CameraController::OnEvent(Event& Event)
	{
		if (m_ControlledCamera)
		{
			m_ControlledCamera->OnEvent(Event);
		}
	}

	void CameraController::OnTick(double DeltaTime)
	{
		if (m_ControlledCamera)
		{
			m_ControlledCamera->OnTick(DeltaTime);
		}
	}
}
