#pragma once

#include "Allocator.hpp"

#pragma warning(disable : 4595)
#pragma warning(disable : 4005)

__forceinline void* operator new(size_t Size)
{
	return Engine::Allocator::Allocate(Size);
}

__forceinline void* operator new[](size_t Size)
{
	return Engine::Allocator::Allocate(Size);
}

__forceinline void operator delete(void* Block, size_t Size)
{
	Engine::Allocator::Deallocate(Block, Size);
}

__forceinline void operator delete[](void* Block, size_t Size)
{
	Engine::Allocator::Deallocate(Block, Size);
}

#pragma warning(default : 4595)