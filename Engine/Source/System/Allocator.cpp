#include "EnginePCH.hpp"

#include "Allocator.hpp"
#include "Core/Debug/Asserts.hpp"
#include "MemoryManager.hpp"

namespace Engine
{

	void* Allocator::Allocate(size_t Size)
	{
#ifndef NDEBUG
		Check(Size < (uint64_t)(1024 * 1024 * 1024));

		auto& memManager = g_MemoryManager.GetMutableMemoryStats();
		memManager.TotalAllocations++;
		memManager.TotalAllocated += Size;
		memManager.CurrentlyAllocated += Size;

		void* buff = malloc(Size + sizeof(size_t));
		*reinterpret_cast<size_t*>(buff) = Size;

		return ((size_t*)buff) + 1;
#else
		return malloc(Size);
#endif
	}

	void Allocator::Deallocate(void* Memory)
	{
#ifndef NDEBUG
		if (Memory)
		{
			size_t size = *(reinterpret_cast<size_t*>(Memory) - 1);

			auto& memManager = g_MemoryManager.GetMutableMemoryStats();
			memManager.TotalFreed += size;
			memManager.CurrentlyAllocated -= size;

			free((void*)((reinterpret_cast<size_t*>(Memory)) - 1));
		}
#else
		free(Memory);
#endif
	}
}
