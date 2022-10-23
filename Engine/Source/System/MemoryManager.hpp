#pragma once

namespace Engine 
{
	struct MemoryStatistics
	{
		uint64_t TotalAllocated;
		uint64_t TotalFreed;
		uint64_t TotalAllocations;
		uint64_t CurrentlyAllocated;

		void LogManager(void) const;
	};

	class MemoryManager final
	{
		friend class Allocator;

	private:
		MemoryStatistics m_MemoryStats {};

		MemoryStatistics& GetMutableMemoryStats(void)
		{
			return m_MemoryStats;
		}

	public:
		MemoryManager() = default;

		MemoryStatistics const& GetMemoryStats(void) const
		{
			return m_MemoryStats;
		}
	};

	inline MemoryManager g_MemoryManager{};
}