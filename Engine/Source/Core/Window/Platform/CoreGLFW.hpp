#pragma once

#include "GLFW/glfw3.h"

#include <cstdint>
#include <iostream>

namespace Engine::Core
{
	namespace GLFW
	{
		constexpr int8_t CONTEXT_VERSION_MAJOR = 4;
		constexpr int8_t CONTEXT_VERSION_MINOR = 0;

		constexpr int8_t WINDOW_RESIZEABLE = 0;
	}

	FORCEINLINE void GLFWErrorCallback(int ErrorCode, const char* Description)
	{
		LOG(Core, Error, "GLFW error ocurred [ {0} ]", Description);
	}
}