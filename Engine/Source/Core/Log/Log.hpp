#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../CoreMinimal.hpp"

namespace Engine
{
	class Log
	{
	public:
		enum class Verbosity : uint8_t
		{
			Trace,Info,Warning,Error
		};

	private:
		using Logger = spdlog::logger;

		static inline const char* s_InitialPattern = "%^[%T] %n: %v%$";

		static std::unordered_map<	std::string_view, 
									std::shared_ptr<Logger>> s_Loggers;

	public:
		static void RegisterLogger(std::string_view const& CategoryName, std::shared_ptr<Logger> Logger);
		
		static std::shared_ptr<Logger>& GetLogger(std::string_view const& CategoryName);
	};
	
	#define LOG(LogCategory,LogVerbosity,...) \
		switch(Log::Verbosity::##LogVerbosity) \
		{\
			case Log::Verbosity::Trace:		Engine::Log::GetLogger(#LogCategory)->trace(__VA_ARGS__);	break; \
			case Log::Verbosity::Info:		Engine::Log::GetLogger(#LogCategory)->info(__VA_ARGS__);	break; \
			case Log::Verbosity::Warning:	Engine::Log::GetLogger(#LogCategory)->warn(__VA_ARGS__);	break; \
			case Log::Verbosity::Error:		Engine::Log::GetLogger(#LogCategory)->error(__VA_ARGS__);	break; \
		}
}