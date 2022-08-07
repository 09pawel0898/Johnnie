#pragma once

#include <stdint.h>
#include <iostream>
#include <optional>

namespace Engine 
{
	struct MemoryStatistics
	{
		uint64_t TotalAllocated;
		uint64_t TotalFreed;
		uint64_t TotalAllocations;
		uint64_t CurrentlyAllocated;

		void Log(void) const;
	};

	class MemoryManager
	{
	private:
		MemoryStatistics m_MemoryStats {};

	public:
		MemoryManager() 
		{}

		MemoryStatistics& GetMutableMemoryStats(void)
		{
			return m_MemoryStats;
		}

		MemoryStatistics const& GetMemoryStats(void) const
		{
			return m_MemoryStats;
		}
	};

	inline MemoryManager g_MemoryManager{};
}