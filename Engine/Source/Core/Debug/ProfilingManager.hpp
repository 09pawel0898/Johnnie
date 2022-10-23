#pragma once

#include "Core/CoreMinimal.hpp"
#include <chrono>
#include <thread>
#include <unordered_map>

namespace Engine
{
	struct ProfileResult
	{
		std::string_view			Name;
		std::chrono::microseconds	Duration;
		std::thread::id				ThreadID;
	};

	class ProfilingManager final
	{
	private:
		static TUniquePtr<ProfilingManager> s_Instance;

	public:
		static TUniquePtr<ProfilingManager>& Get(void)
		{
			if (!s_Instance)
			{
				s_Instance = MakeUnique<ProfilingManager>();
			}
			return s_Instance;
		}

	public:
		std::unordered_map<std::string_view, ProfileResult> m_ProfileResults;

		void BeginScopeProfile(std::string_view Name);

		void UpdateScopeProfile(std::string_view Name, std::chrono::microseconds Duration);

		ProfileResult const& GetResult(std::string_view Name);
	};
}