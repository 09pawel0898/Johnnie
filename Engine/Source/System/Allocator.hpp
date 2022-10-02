#pragma once

namespace Engine
{
	class Allocator
	{
	public:
		[[nodiscard]]
		static void* Allocate(size_t Size);

		static void Deallocate(void* Memory);
	};
}