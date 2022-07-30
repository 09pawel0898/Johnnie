#pragma once

namespace Engine
{
	class Allocator
	{
	public:
		static void* Allocate(size_t Size);
		static void Free(void* Memory, size_t Size);
	};
}