#include "EnginePCH.hpp"

#include "Allocator.hpp"
#include "Core/Debug/Asserts.hpp"
#include "MemoryManager.hpp"


void* Engine::Allocator::Allocate(size_t Size)
{
	Check(Size < (uint64_t)(1024 * 1024 * 1024));

	auto& memManager = g_MemoryManager.GetMutableMemoryStats();
	memManager.TotalAllocations++;
	memManager.TotalAllocated += Size;
	memManager.CurrentlyAllocated += Size;
	
	std::byte* result = static_cast<std::byte*>(malloc(Size));
	return result;
}

void* Engine::Allocator::AllocateArray(size_t Size)
{
	Check(Size < (uint64_t)(1024 * 1024 * 1024));

	auto& memManager = g_MemoryManager.GetMutableMemoryStats();
	memManager.TotalAllocations++;
	memManager.TotalAllocated += Size;
	memManager.CurrentlyAllocated += Size;

	void* buff = malloc(Size + sizeof(size_t));
	*reinterpret_cast<size_t*>(buff) = Size;

	return ((size_t*)buff) + 1;
}

void Engine::Allocator::Deallocate(void* Memory, size_t Size)
{
	auto& memManager = g_MemoryManager.GetMutableMemoryStats();
	memManager.TotalFreed += Size;
	memManager.CurrentlyAllocated -= Size;
	free(Memory);
}

void Engine::Allocator::DeallocateArray(void* Memory)
{
	if (Memory)
	{
		size_t size = *(reinterpret_cast<size_t*>(Memory) - 1);

		auto& memManager = g_MemoryManager.GetMutableMemoryStats();
		memManager.TotalFreed += size;
		memManager.CurrentlyAllocated -= size;

		free((void*)(((size_t*)Memory) - 1));
	}
}

