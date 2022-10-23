#pragma once

#include "Core/CoreMinimal.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "OutputLogSink.hpp"

namespace Engine
{
	namespace Core
	{
		class Application;
	}

	class LogManager
	{
		friend class Core::Application;

	public:
		enum class Verbosity : uint8_t
		{
			Trace,Info,Warning,Error
		};

	private:
		using Logger = spdlog::logger;

		static inline const char* s_DefaultPattern = "%^[%T][%n][%l]:  %v%$";

		static std::unordered_map<	std::string_view, 
									TSharedPtr<Logger>> s_Loggers;

	public:
		static void RegisterLogger(std::string_view const& CategoryName, std::shared_ptr<Logger> Logger);

		static TSharedPtr<Logger>& GetLogger(std::string_view const& CategoryName);

		static void RegisterOutputLogSink_mt(TSharedPtr<OutputLogSink_mt> OutputLogSink_mt);
		
		static const char* GetDefaultPattern(void)
		{
			return s_DefaultPattern;
		}

	private:
		static void RegisterEngineLoggers(void);
	};
	
	#define DEFINE_LOG_CATEGORY(LogCategory)\
		LogManager::RegisterLogger(#LogCategory,spdlog::stdout_color_mt(#LogCategory))

	#define DEFINE_OUTPUT_LOG_SINK(_OutputLogSink)\
		LogManager::RegisterOutputLogSink_mt(_OutputLogSink)

	#define LOG(LogCategory,LogVerbosity,...) \
		switch(LogManager::Verbosity::##LogVerbosity) \
		{\
			case LogManager::Verbosity::Trace:		LogManager::GetLogger(#LogCategory)->trace(__VA_ARGS__);	break; \
			case LogManager::Verbosity::Info:		LogManager::GetLogger(#LogCategory)->info(__VA_ARGS__);	break; \
			case LogManager::Verbosity::Warning:	LogManager::GetLogger(#LogCategory)->warn(__VA_ARGS__);	break; \
			case LogManager::Verbosity::Error:		LogManager::GetLogger(#LogCategory)->error(__VA_ARGS__);	break; \
		}
}