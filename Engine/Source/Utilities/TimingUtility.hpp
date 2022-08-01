#pragma once

#include "Log/Log.hpp"

#include <chrono>
#include <utility>
#include <type_traits>

#include <algorithm>
#include <future>

#include <concepts>

namespace Engine::Utility
{
	template<typename TimeUnit>
	struct is_duration : std::false_type {};

	template<typename Rep, typename Period>
	struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};

	template <typename TimeUnit>
		requires is_duration<TimeUnit>::value
	constexpr std::string_view TimeUnitToStringV(){ static_assert(false); }

	template <> constexpr std::string_view TimeUnitToStringV<std::chrono::milliseconds>()	{ return "ms"; }
	template <> constexpr std::string_view TimeUnitToStringV<std::chrono::microseconds>()	{ return "us"; }
	template <> constexpr std::string_view TimeUnitToStringV<std::chrono::seconds>()		{ return "s"; }

	template <typename TimeUnit = std::chrono::milliseconds>
		requires is_duration<TimeUnit>::value
	class Timer
	{
	private:
		using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	private:
		std::string_view	m_OnEndMessage;
		TimePoint			m_StartTimepoint;

	public:

		explicit Timer(std::string_view OnEndMessage = "Timer finished execution")
			:	m_OnEndMessage(OnEndMessage)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			TimePoint endTimepoint = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::time_point_cast<TimeUnit>(endTimepoint).time_since_epoch() -
							std::chrono::time_point_cast<TimeUnit>(m_StartTimepoint).time_since_epoch();
			auto _duration = duration.count();

			LOG(Core,Trace,"{0} : {1} {2}", m_OnEndMessage,_duration, TimeUnitToStringV<TimeUnit>())
		}

		Timer(const Timer& rhs) = delete;
		Timer(Timer&& rhs)		= delete;

		Timer& operator =(const Timer& rhs) = delete;
		Timer& operator =(Timer&& rhs)		= delete;
	};
	
	template <typename TimeUnit = std::chrono::milliseconds, typename Invocable, typename... Args>
		requires std::invocable<Invocable,Args...> && is_duration<TimeUnit>::value
	void ExecuteAndMeasure(std::string_view OnEndMessage, Invocable&& Func, Args&&... Params)
	{
		std::string_view onEndMessage = OnEndMessage;
		auto startTimepoint = std::chrono::high_resolution_clock::now();

		std::forward<decltype(Func)>(Func)(
			std::forward<decltype(Params)>(Params)...);

		auto endTimepoint = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::time_point_cast<TimeUnit>(endTimepoint).time_since_epoch() -
						std::chrono::time_point_cast<TimeUnit>(startTimepoint).time_since_epoch();
		auto _duration = duration.count();
		
		LOG(Core, Trace, "{0} : {1} {2}", onEndMessage, _duration, TimeUnitToStringV<TimeUnit>())
	}
}