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
		// Nvidia
		int32_t total_video;    // dedicated video memory, total size (in kb) of the GPU memory
		int32_t total_system;   // total available memory, total size (in Kb) of the memory available for allocations
		int32_t gpu_available;  // current available dedicated video memory (in kb), currently unused GPU memory

		// Ati
		int32_t total_pool;     // total memory free in the pool
		int32_t largest_block;  // largest available free block in the pool
		int32_t total_aux;      // total auxiliary memory free
		int32_t largest_aux;    // largest auxiliary free block
	};

	// a value of -1 means it is not supported or not set
	struct SystemVideoMemoryEvictionInfo
	{
		int32_t count;  // count of total evictions seen by system
		int32_t size;   // size of total video memory evicted (in kb)
	};

	class System
	{
	private:
		static inline bool s_bInitialized = false;

	public:
		static void Init(void);
		static void Shutdown(void);

	private:
		static void InitVideoMemoryInfo(void);

	public:		
		/** Physical and virtual memory info */
		static std::optional<SystemMemoryInfo> GetMemoryInfo(void);

		/** Video memory info */
		static std::optional<SystemVideoMemoryInfo> GetVideoMemoryInfo(void);

		/** Allocated memory info */
		static MemoryStatistics const& GetMemoryStatistics(void);
	};
}