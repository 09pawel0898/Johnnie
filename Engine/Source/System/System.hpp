#pragma once

#include <optional>
#include "MemoryManager.hpp"

namespace Engine
{
	struct SystemMemoryInfo
	{
		uint64_t AvailablePhysMemory;
		uint64_t TotalPhysMemory;

		uint64_t AvailableVirtualMemory;
		uint64_t TotalVirtualMemory;

		void Log(void) const;
	};
	
	struct SystemVideoMemoryInfo
	{

	};

	class System
	{
	public:
		/** Physical and virtual memory info */
		static std::optional<SystemMemoryInfo> GetMemoryInfo(void);

		/** Video memory info */
		static std::optional<SystemVideoMemoryInfo> GetVideoMemoryInfo(void);

		/** Allocated memory info */
		static MemoryStatistics const& GetMemoryStatistics(void);
	};
}