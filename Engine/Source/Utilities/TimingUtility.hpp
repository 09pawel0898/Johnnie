#pragma once

#include "Core/CoreMinimal.hpp"
#include "Core/Debug/ProfilingManager.hpp"

#include <chrono>
#include <utility>
#include <type_traits>

#include <algorithm>
#include <future>

#include <concepts>

namespace Engine::Utility
{
	using Duration = std::chrono::duration<double>;
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using Time = std::chrono::high_resolution_clock;

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
		std::string_view	m_ProfileName;
		TimePoint			m_StartTimepoint;

	public:

		explicit Timer(std::string_view Name)
			:	m_ProfileName(Name)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			TimePoint endTimepoint = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::time_point_cast<TimeUnit>(endTimepoint).time_since_epoch() -
							std::chrono::time_point_cast<TimeUnit>(m_StartTimepoint).time_since_epoch();
			auto _duration = duration.count();

#if PROFILE_RESULT_LOG
			LOG(Profile, Trace, "{0} : {1} {2}", m_ProfileName, _duration, TimeUnitToStringV<TimeUnit>())
#endif
			ProfilingManager::Get()->UpdateScopeProfile(m_ProfileName, duration);
		}

		Timer(const Timer& rhs) = delete;
		Timer(Timer&& rhs)		= delete;

		Timer& operator =(const Timer& rhs) = delete;
		Timer& operator =(Timer&& rhs)		= delete;
	};
	
	template <typename TimeUnit = std::chrono::milliseconds, typename Invocable, typename... Args>
		requires std::invocable<Invocable,Args...> && is_duration<TimeUnit>::value
	void ExecuteAndMeasure(std::string_view ProfileName, Invocable&& Func, Args&&... Params)
	{
		auto startTimepoint = std::chrono::high_resolution_clock::now();

		std::forward<decltype(Func)>(Func)(
			std::forward<decltype(Params)>(Params)...);

		auto endTimepoint = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::time_point_cast<TimeUnit>(endTimepoint).time_since_epoch() -
						std::chrono::time_point_cast<TimeUnit>(startTimepoint).time_since_epoch();
		auto _duration = duration.count();
		
#if PROFILE_RESULT_LOG
		LOG(Profile, Trace, "{0} : {1} {2}", ProfileName, _duration, TimeUnitToStringV<TimeUnit>())
#endif
		ProfilingManager::Get()->UpdateScopeProfile(ProfileName, duration);
	}
}