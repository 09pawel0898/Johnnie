#include "MemoryManager.hpp"

#include "Utilities/Log.hpp"
#include "Utilities/StringUtility.hpp"

namespace Engine
{
	void MemoryStatistics::Log(void) const
	{
		LOG(Core, Trace, "----------------------------Memory Statistics----------------------------");
		
		LOG(Core, Trace, "\tTotal allocated :\t{0} ",		Utility::BytesToString(TotalAllocated));
		LOG(Core, Trace, "\tCurrently allocated :\t{0} ",	Utility::BytesToString(CurrentlyAllocated));
		LOG(Core, Trace, "\tTotal freed :\t\t{0} ",			Utility::BytesToString(TotalFreed));
		LOG(Core, Trace, "\tTotal allocations :\t{0} ",		Utility::BytesToString(TotalAllocations));
		
		LOG(Core, Trace, "-------------------------------------------------------------------------");
	}
}