#pragma once

#include "Core/CoreMinimal.hpp"
#include "Events/IEventListener.hpp"
#include "Utilities/Singleton.hpp"

namespace Engine
{
	class OCamera;

	using namespace Events;

	class CameraController : public IEventListener, Singleton<CameraController>
	{
	private:
		std::shared_ptr<OCamera> m_ManagedCamera;
	public:

		virtual void OnEvent(Event& Event) override;
	};
}