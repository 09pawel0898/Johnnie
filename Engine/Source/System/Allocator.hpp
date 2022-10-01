#pragma once

namespace Engine
{
	class Allocator
	{
	public:
		[[nodiscard]]
		static void* Allocate(size_t Size);
		[[nodiscard]]
		static void* AllocateArray(size_t Size);

		static void Deallocate(void* Memory, size_t Size);
		static void DeallocateArray(void* Memory);
	};
}