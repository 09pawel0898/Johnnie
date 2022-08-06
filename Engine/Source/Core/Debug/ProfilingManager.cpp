#include "ProfilingManager.hpp"
#include "Core/Debug/Asserts.hpp"

namespace Engine
{
	std::unique_ptr<ProfilingManager> ProfilingManager::s_Instance = nullptr;

	void ProfilingManager::BeginScopeProfile(std::string_view Name)
	{
		if (m_ProfileResults.find(Name) == m_ProfileResults.end())
		{
			return;
		}

		ProfileResult& result = m_ProfileResults[Name];
		result.Name = Name;
		result.ThreadID = std::this_thread::get_id();
	}

	void ProfilingManager::UpdateScopeProfile(std::string_view Name, std::chrono::milliseconds Duration)
	{
		m_ProfileResults[Name].Duration = Duration;
	}

	ProfileResult const& ProfilingManager::GetResult(std::string_view Name)
	{
		return m_ProfileResults[Name];
	}
}