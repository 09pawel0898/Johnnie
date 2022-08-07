#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "OutputLogSink.hpp"

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

		static inline const char* s_InitialPattern = "%^[%T][%n][%l]:  %v%$";

		static std::unordered_map<	std::string_view, 
									std::shared_ptr<Logger>> s_Loggers;

	public:
		static const char* GetDefaultPattern(void) 
		{ 
			return s_InitialPattern; 
		}

		static void RegisterLogger(std::string_view const& CategoryName, std::shared_ptr<Logger> Logger);

		static void RegisterOutputLogSink_mt(std::shared_ptr<OutputLogSink_mt> OutputLogSink_mt);

		static std::shared_ptr<Logger>& GetLogger(std::string_view const& CategoryName);

	public:
		static void RegisterEngineLoggers(void);
	};
	
	#define DEFINE_LOG_CATEGORY(LogCategory)\
		Log::RegisterLogger(#LogCategory,spdlog::stdout_color_mt(#LogCategory))

	#define DEFINE_OUTPUT_LOG_SINK(_OutputLogSink)\
		Log::RegisterOutputLogSink_mt(_OutputLogSink)

	#define LOG(LogCategory,LogVerbosity,...) \
		switch(Log::Verbosity::##LogVerbosity) \
		{\
			case Log::Verbosity::Trace:		Log::GetLogger(#LogCategory)->trace(__VA_ARGS__);	break; \
			case Log::Verbosity::Info:		Log::GetLogger(#LogCategory)->info(__VA_ARGS__);	break; \
			case Log::Verbosity::Warning:	Log::GetLogger(#LogCategory)->warn(__VA_ARGS__);	break; \
			case Log::Verbosity::Error:		Log::GetLogger(#LogCategory)->error(__VA_ARGS__);	break; \
		}
}