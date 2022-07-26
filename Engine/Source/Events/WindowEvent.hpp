#pragma once

#include "Event.hpp"

namespace Engine::Events
{
	class WindowClosedEvent final : public Event
	{
	public:
		WindowClosedEvent() = default;
		
		DECLARE_EVENT_CLASS_TYPE(WindowClosed);
		DECLARE_EVENT_CATEGORY(EventCategory_Application)
	};
}