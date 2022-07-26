#pragma once

#include "EventsIdentifiers.hpp"
#include "../Utilities/FlagOperators.hpp"

#include <functional>

namespace Engine::Events
{
	#define DECLARE_EVENT_CLASS_TYPE(Type)\
		static	EventType GetStaticType(void)				{ return EventType::Type; }\
		virtual EventType GetEventType(void) const override { return GetStaticType(); }

	#define DECLARE_EVENT_CATEGORY(Category)\
		virtual EventCategory GetCategoryFlags() const override { return Category; }

	class Event
	{
	public:
		bool m_Handled = false;
	
	public:
		virtual ~Event() = default;
		
		virtual EventType GetEventType() const = 0;
		virtual EventCategory GetCategoryFlags() const = 0;

		bool HasCategory(EventCategory Category)
		{
			return Category & GetCategoryFlags();
		}

		inline bool Handled() const { return m_Handled; }
	};

	class EventDispatcher
	{
		template<typename EventType>
		using EventCallback = std::function<bool(EventType&)>;

	private:
		Event& m_Event;
	
	public:
		explicit EventDispatcher(Event& Event) noexcept
			:	m_Event(Event)
		{}

		template<typename EventType>
			requires std::is_base_of_v<Event,EventType>
		bool Dispatch(EventCallback<EventType> const& Func)
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