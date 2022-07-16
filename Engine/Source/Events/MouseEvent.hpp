#pragma once

#include "Event.hpp"

namespace Engine::Events
{
	namespace Mouse
	{
		enum class MouseButtonCode : uint16_t
		{
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			ButtonLeft = Button0,
			ButtonRight = Button1
		};
	}
	using namespace Mouse;

	class MouseMovedEvent : public Event
	{
	private:
		double m_MouseX, m_MouseY;

	public:
		MouseMovedEvent(double X, double Y)
			:	m_MouseX(X),
				m_MouseY(Y)
		{}

		inline double GetX(void) const { return m_MouseX; }
		inline double GetY(void) const { return m_MouseY; }

		DECLARE_EVENT_CLASS_TYPE(MouseMoved)
	};

	class MouseButtonEvent : public Event
	{
	protected:
		MouseButtonCode m_ButtonCode;

		explicit MouseButtonEvent(MouseButtonCode Button)
			:	m_ButtonCode(Button)
		{}

	public:
		inline MouseButtonCode GetButtonCode(void) const
		{
			return m_ButtonCode;
		}
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressedEvent(MouseButtonCode Button)
			:	MouseButtonEvent(Button)
		{}

		DECLARE_EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleasedEvent(MouseButtonCode Button)
			:	MouseButtonEvent(Button)
		{}

		DECLARE_EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}