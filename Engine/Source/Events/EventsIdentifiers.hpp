#pragma once

#include "../Core/CoreMinimal.hpp"

namespace Engine::Events
{
	enum class EventType : uint8_t
	{
		None = 0,

		// Key Events
		KeyPressed,
		KeyReleased,

		// Mouse Events
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		
		// Window Events
		WindowClosed
	};
}