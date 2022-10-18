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

	class WindowResizedEvent final : public Event
	{
	private:
		int32_t m_Width{ 0 };
		int32_t m_Heigth{ 0 };
	
	public:
		WindowResizedEvent(int32_t Width, int32_t Heigth)
			:	m_Width(Width),m_Heigth(Heigth)
		{}

		int32_t GetWidth(void) const
		{
			return m_Width;
		}

		int32_t GetHeigth(void) const
		{
			return m_Heigth;
		}

		DECLARE_EVENT_CLASS_TYPE(WindowResized);
		DECLARE_EVENT_CATEGORY(EventCategory_Application)
	};
}