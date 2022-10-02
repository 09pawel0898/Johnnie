#pragma once

#include "Allocator.hpp"

#pragma warning(disable : 4595)
#pragma warning(disable : 4005)

[[nodiscard]]
__forceinline void* operator new(size_t Size)
{
	return Engine::Allocator::Allocate(Size);
}

[[nodiscard]]
__forceinline void* operator new[](size_t Size)
{
	return Engine::Allocator::Allocate(Size);
}

__forceinline void operator delete(void* Block, size_t Size)
{
	Engine::Allocator::Deallocate(Block);
}

__forceinline void operator delete(void* Block)
{
	Engine::Allocator::Deallocate(Block);
}

__forceinline void operator delete[](void* Block, size_t Size)
{
	Engine::Allocator::Deallocate(Block);
}

__forceinline void operator delete[](void* Block)
{
	Engine::Allocator::Deallocate(Block);
}

#pragma warning(default : 4595)