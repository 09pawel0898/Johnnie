#pragma once

#include "../../Events/KeyEvent.hpp"
#include "../../Events/MouseEvent.hpp"

#include <glm/glm.hpp>

namespace Engine
{
	class Input
	{
	public:
		static glm::vec2 GetMousePosition(void);
		
		static void SetMousePosition(glm::vec2 MousePos);

		static bool IsKeyPressed(Events::KeyCode KeyCode);

		static bool IsMouseButtonPressed(Events::MouseButtonCode MouseButtonCode);
	};
}