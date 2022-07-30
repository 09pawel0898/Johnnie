#include "Allocator.hpp"
#include "Core/Debug/Asserts.hpp"
#include "MemoryManager.hpp"

#include <cstddef>

void* Engine::Allocator::Allocate(size_t Size)
{
    CheckMsg(Size < 1024 * 1024 * 1024, "Memory block is to big to allocate!");
	
	MemoryManager::Get()->GetMemoryStats().TotalAllocations++;
	MemoryManager::Get()->GetMemoryStats().TotalAllocated += Size;
	MemoryManager::Get()->GetMemoryStats().CurrentlyAllocated += Size;
	
	std::byte* result = (std::byte*)malloc(Size);
	return result;
}

void Engine::Allocator::Free(void* Memory, size_t Size)
{
	MemoryManager::Get()->GetMemoryStats().TotalFreed += Size;
	MemoryManager::Get()->GetMemoryStats().CurrentlyAllocated -= Size;
	free(Memory);
}
