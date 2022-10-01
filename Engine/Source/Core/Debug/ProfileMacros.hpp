#pragma once

#include "ProfilingManager.hpp"
#include "Utilities/TimingUtility.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////

#define __PROFILE_SCOPE(UniqueName,Line)\
	Engine::ProfilingManager::Get()->BeginScopeProfile(##UniqueName);\
	Engine::Utility::Timer<std::chrono::microseconds> timer##Line(##UniqueName);


#define PROFILE_SCOPE(UniqueName)\
	__PROFILE_SCOPE(UniqueName,__LINE__)

//////////////////////////////////////////////////////////////////////////////////////////////

#define PROFILE_STATIC_FUNCTION(UniqueName, Func, ...)\
	Engine::ProfilingManager::Get()->BeginScopeProfile(##UniqueName);\
	Engine::Utility::ExecuteAndMeasure(##UniqueName, ##Func, __VA_ARGS__);


#define PROFILE_MEMBER_FUNCTION(UniqueName, Func, ...)\
	Engine::ProfilingManager::Get()->BeginScopeProfile(##UniqueName);\
	Engine::Utility::ExecuteAndMeasure(##UniqueName, [this](){this->##Func(__VA_ARGS__);})

//////////////////////////////////////////////////////////////////////////////////////////////

/** Returns last profile result in microseconds */
#define GET_PROFILE_RESULT(Name)\
	ProfilingManager::Get()->GetResult(##Name).Duration.count()

//////////////////////////////////////////////////////////////////////////////////////////////