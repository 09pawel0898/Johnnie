#include "EnginePCH.hpp"

#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../Core/CoreMinimal.hpp"

namespace Engine
{
	std::unordered_map<std::string_view,TSharedPtr<LogManager::Logger>> LogManager::s_Loggers;
	
	void LogManager::RegisterLogger(std::string_view const& CategoryName, std::shared_ptr<Logger> Logger)
	{
		auto logger = s_Loggers.find(CategoryName);
		CheckMsg(logger == s_Loggers.end(), "Logger with name provided is already registered.");

		s_Loggers[CategoryName] = MoveTemp(Logger);
		s_Loggers[CategoryName]->set_pattern(s_DefaultPattern);
		s_Loggers[CategoryName]->set_level(spdlog::level::trace);
	}

	void LogManager::RegisterOutputLogSink_mt(TSharedPtr<OutputLogSink_mt> OutputLogSink_mt)
	{
		for (auto& [loggerName, logger] : s_Loggers)
		{
			OutputLogSink_mt->set_pattern(s_DefaultPattern);
			OutputLogSink_mt->set_level(spdlog::level::trace);

			logger->sinks().emplace_back(OutputLogSink_mt);
		}
	}

	TSharedPtr<LogManager::Logger>& LogManager::GetLogger(std::string_view const& CategoryName)
	{
		auto logger = s_Loggers.find(CategoryName);
		CheckMsg(logger != s_Loggers.end(),"Logger with provided name was not registered.");
		
		return logger->second;
	}

	void LogManager::RegisterEngineLoggers(void)
	{
		DEFINE_LOG_CATEGORY(Core);
		DEFINE_LOG_CATEGORY(Events);
		DEFINE_LOG_CATEGORY(Profile);
		DEFINE_LOG_CATEGORY(RHI);
		DEFINE_LOG_CATEGORY(Assimp);
		DEFINE_LOG_CATEGORY(Materials);
		DEFINE_LOG_CATEGORY(Shaders);
	}
}