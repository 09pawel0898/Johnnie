#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine
{
	std::shared_ptr<Log::Logger> Log::s_CoreLogger;
	std::shared_ptr<Log::Logger> Log::s_ClientLogger;
	
	void Log::Init(void)
	{
		const std::string initialPattern = "%^[%T] %n: %v%$";

		s_CoreLogger = spdlog::stdout_color_mt("Engine");
		s_CoreLogger->set_pattern(initialPattern);
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Client");
		s_ClientLogger->set_pattern(initialPattern);
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}