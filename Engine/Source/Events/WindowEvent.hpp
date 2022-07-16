#pragma once

#include "Event.hpp"

namespace Engine::Events
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;
		
		DECLARE_EVENT_CLASS_TYPE(WindowClosed);
	};
}