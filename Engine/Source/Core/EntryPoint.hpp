#pragma once

#include "Application/Application.hpp"
#include "Utilities/Log.hpp"
#include "Exceptions/InitializationException.hpp"
#include "PlatformCheck.hpp"

using namespace Engine;

extern std::shared_ptr<Core::Application> Core::CreateApplication(void);

auto main(void) -> int
{
	try
	{
		Core::Application::s_Instance = Core::CreateApplication();
		Core::Application::s_Instance->PostInitApplication();
		Core::Application::s_Instance->Run();
	}
	catch (Engine::InitializationException e)
	{
		LOG(Core, Error, e.what());
	}
	return 0;
}