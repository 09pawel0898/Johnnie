#include "MemoryManager.hpp"

#include "Utilities/Log.hpp"

namespace Engine
{
	std::unique_ptr<MemoryManager> MemoryManager::s_Instance = nullptr;

	std::unique_ptr<MemoryManager> const& MemoryManager::Get()
	{
		if (!s_Instance)
		{
			s_Instance = std::make_unique<MemoryManager>();
		}
		return s_Instance;
	}
}