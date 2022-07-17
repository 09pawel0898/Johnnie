#pragma once

#include <spdlog/spdlog.h>

namespace Engine
{
	class Log
	{
		using Logger = spdlog::logger;

	private:
		static std::shared_ptr<Logger> s_CoreLogger;
		static std::shared_ptr<Logger> s_ClientLogger;

	public:
		inline static std::shared_ptr<Logger>& GetCoreLogger(void)
		{
			return s_CoreLogger;
		}
		inline static std::shared_ptr<Logger>& GetClientLogger(void)
		{
			return s_ClientLogger;
		}

		static void Init(void);
	};
}