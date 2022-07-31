#include "Allocator.hpp"
#include "Core/Debug/Asserts.hpp"
#include "MemoryManager.hpp"

#include <cstddef>

void* Engine::Allocator::Allocate(size_t Size)
{
    Check(Size < 1024 * 1024 * 1024);
	
	g_MemoryManager.GetMutableMemoryStats().TotalAllocations++;
	g_MemoryManager.GetMutableMemoryStats().TotalAllocated += Size;
	g_MemoryManager.GetMutableMemoryStats().CurrentlyAllocated += Size;
	
	std::byte* result = static_cast<std::byte*>(malloc(Size));
	return result;
}

void Engine::Allocator::Deallocate(void* Memory, size_t Size)
{
	g_MemoryManager.GetMutableMemoryStats().TotalFreed += Size;
	g_MemoryManager.GetMutableMemoryStats().CurrentlyAllocated -= Size;
	free(Memory);
}
