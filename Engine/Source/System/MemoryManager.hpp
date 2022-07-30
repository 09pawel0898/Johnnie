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
	};

	class MemoryManager
	{
	private:
		MemoryStatistics m_MemoryStats;

	public:
		MemoryManager() = default;
		
		inline MemoryStatistics& GetMemoryStats()
		{
			return m_MemoryStats;
		}

		inline MemoryStatistics const& GetMemoryStats() const
		{
			return m_MemoryStats;
		}

	private:
		static std::unique_ptr<MemoryManager> s_Instance;
	public:
		static std::unique_ptr<MemoryManager> const& Get();
	};
}