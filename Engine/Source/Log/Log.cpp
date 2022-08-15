#include "EnginePCH.hpp"

#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../Core/CoreMinimal.hpp"

namespace Engine
{
	std::unordered_map<std::string_view,std::shared_ptr<Log::Logger>> Log::s_Loggers;
	
	void Log::RegisterLogger(std::string_view const& CategoryName, std::shared_ptr<Logger> Logger)
	{
		auto logger = s_Loggers.find(CategoryName);
		CheckMsg(logger == s_Loggers.end(), "Logger with name provided is already registered.");
		s_Loggers[CategoryName] = std::move(Logger);
		s_Loggers[CategoryName]->set_pattern(s_InitialPattern);
		s_Loggers[CategoryName]->set_level(spdlog::level::trace);
	}

	void Log::RegisterOutputLogSink_mt(std::shared_ptr<OutputLogSink_mt> OutputLogSink_mt)
	{
		for (auto& [loggerName, logger] : s_Loggers)
		{
			OutputLogSink_mt->set_pattern(s_InitialPattern);
			OutputLogSink_mt->set_level(spdlog::level::trace);

			logger->sinks().emplace_back(OutputLogSink_mt);
		}
	}

	std::shared_ptr<Log::Logger>& Log::GetLogger(std::string_view const& CategoryName)
	{
		auto logger = s_Loggers.find(CategoryName);
		CheckMsg(logger != s_Loggers.end(),"Logger with provided name was not registered.");
		
		return logger->second;
	}

	void Log::RegisterEngineLoggers(void)
	{
		DEFINE_LOG_CATEGORY(Core);
		DEFINE_LOG_CATEGORY(Events);
		DEFINE_LOG_CATEGORY(Profile);
		DEFINE_LOG_CATEGORY(RHI);
	}
}