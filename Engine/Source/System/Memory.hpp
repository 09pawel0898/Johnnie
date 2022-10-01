#pragma once

#include "Allocator.hpp"

#pragma warning(disable : 4595)
#pragma warning(disable : 4005)

[[nodiscard]]
__forceinline void* operator new(size_t Size)
{
	return Engine::Allocator::AllocateArray(Size);
}

[[nodiscard]]
__forceinline void* operator new[](size_t Size)
{
	return Engine::Allocator::AllocateArray(Size);
}

__forceinline void operator delete(void* Block, size_t Size)
{
	Engine::Allocator::DeallocateArray(Block);
}

__forceinline void operator delete(void* Block)
{
	Engine::Allocator::DeallocateArray(Block);
}

__forceinline void operator delete[](void* Block, size_t Size)
{
	Engine::Allocator::DeallocateArray(Block);
}

__forceinline void operator delete[](void* Block)
{
	Engine::Allocator::DeallocateArray(Block);
}

#pragma warning(default : 4595)