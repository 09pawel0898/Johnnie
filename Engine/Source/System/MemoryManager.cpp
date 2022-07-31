#include "MemoryManager.hpp"

#include "Utilities/Log.hpp"
#include "Utilities/StringUtility.hpp"

namespace Engine
{
	void MemoryStatistics::Log(void) const
	{
		LOG(Core, Trace, "----------------------------Memory Statistics----------------------------");
		
		LOG(Core, Trace, "\tTotal allocated :\t{0} ",		Utilities::BytesToString(TotalAllocated));
		LOG(Core, Trace, "\tCurrently allocated :\t{0} ",	Utilities::BytesToString(CurrentlyAllocated));
		LOG(Core, Trace, "\tTotal freed :\t\t{0} ",			Utilities::BytesToString(TotalFreed));
		LOG(Core, Trace, "\tTotal allocations :\t{0} ",		Utilities::BytesToString(TotalAllocations));
		
		LOG(Core, Trace, "-------------------------------------------------------------------------");
	}
}