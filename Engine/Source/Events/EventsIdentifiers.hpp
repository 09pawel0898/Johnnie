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
		WindowClosed,
		WindowResized
	};

	enum EventCategory : uint8_t
	{
		EventCategory_Application	= 0x00,
		EventCategory_Input			= 0x01,
		EventCategory_Keyboard		= 0x02,
		EventCategory_Mouse			= 0x04,
		EventCategory_MouseButton	= 0x08
	};
}