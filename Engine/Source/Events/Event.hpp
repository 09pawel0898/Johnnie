#pragma once

#include "EventsIdentifiers.hpp"
#include <functional>

namespace Engine::Events
{
	#define DECLARE_EVENT_CLASS_TYPE(type)\
		static	EventType GetStaticType(void)				{ return EventType::type; }\
		virtual EventType GetEventType(void) const override { return GetStaticType(); }

	class Event
	{
	protected:
		bool m_Handled = false;
		friend class EventDispatcher;
	
	public:
		virtual ~Event() = default;
		
		virtual EventType GetEventType() const = 0;
		inline bool Handled() const { return m_Handled; }
	};

	class EventDispatcher
	{
		template<typename EventType>
		using EventFunction = std::function<bool(EventType&)>;

	private:
		Event& m_Event;
	
	public:
		explicit EventDispatcher(Event& Event)
			:	m_Event(Event)
		{}

		template<typename EventType>
		bool Dipatch(EventFunction<EventType> const& Func)
		{
			if (m_Event.GetEventType() == EventType::GetStaticType())
			{
				m_Event.m_Handled = Func(*static_cast<EventType*>(&m_Event));
				return true;
			}
			return false;
		}
	};
}