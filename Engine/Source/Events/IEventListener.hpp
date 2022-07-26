#pragma once

#include "Event.hpp"

namespace Engine::Events
{
	class IEventListener
	{
		virtual void OnEvent(Event& Event) = 0;
	};
}