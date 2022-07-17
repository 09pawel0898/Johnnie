#pragma once

#include "Application.hpp"

using namespace Engine;

extern std::shared_ptr<Core::Application> Core::CreateApplication(void);

int main(void)
{
	Core::Application::s_Instance = Core::CreateApplication();
	Core::Application::s_Instance->Run();
	return 0;
}