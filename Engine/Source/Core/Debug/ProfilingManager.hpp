#pragma once

#include "Core/CoreMinimal.hpp"
#include "Utilities/Singleton.hpp"

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

	class ProfilingManager final : public Singleton<ProfilingManager>
	{
	private:
		std::unordered_map<std::string_view, ProfileResult> m_ProfileResults;

	public:
		void BeginScopeProfile(std::string_view Name);
		void UpdateScopeProfile(std::string_view Name, std::chrono::microseconds Duration);

		ProfileResult const& GetResult(std::string_view Name);
	};
}