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

		void Log() const;
	};
	
	struct SystemVideoMemoryInfo
	{

	};

	class System
	{
	public:
		/** Physical and virtual memory info */
		static std::optional<SystemMemoryInfo> GetMemoryInfo();

		/** Video memory info */
		static std::optional<SystemVideoMemoryInfo> GetVideoMemoryInfo();

		/** Allocated memory info */
		static MemoryStatistics const& GetMemoryStatistics();
	};
}