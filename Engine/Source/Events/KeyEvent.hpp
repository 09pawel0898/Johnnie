#pragma once

#include "Event.hpp"

namespace Engine::Events
{
	namespace Keyboard
	{
		enum class KeyCode : uint16_t
		{
			A = 65, B = 66, C = 67, D = 68, E = 69, F = 70,
			G = 71, H = 72, I = 73, J = 74, K = 75, L = 76,
			M = 77, N = 78, O = 79, P = 70, Q = 81, R = 82, 
			S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
			Y = 89, Z = 90,

			D0 = 48, D1 = 49, D2 = 50, D3 = 51, D4 = 52, 
			D5 = 53, D6 = 54, D7 = 55, D8 = 56, D9 = 57,
		};
	}
	using namespace Keyboard;

	class KeyEvent : public Event
	{
	protected:
		KeyCode m_KeyCode;

		explicit KeyEvent(KeyCode KeyCode) noexcept
			:	m_KeyCode(KeyCode)
		{}

	public:
		inline KeyCode GetKeyCode(void) const
		{
			return m_KeyCode;
		}
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		explicit KeyPressedEvent(KeyCode KeyCode) noexcept
			:	KeyEvent(KeyCode)
		{}

		DECLARE_EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent(KeyCode KeyCode) noexcept
			:	KeyEvent(KeyCode)
		{}

		DECLARE_EVENT_CLASS_TYPE(KeyReleased)
	};
}