#include "System.hpp"

#include "Utilities/Log.hpp"
#include "Utilities/StringUtility.hpp"

#include "Core/PlatformCheck.hpp"

#ifdef PLATFORM_WINDOWS
#	include <Windows.h>
#endif


namespace Engine
{
	void SystemMemoryInfo::Log(void) const
	{
		LOG(Core, Trace, "-------------------------------Memory Info-------------------------------");

		LOG(Core, Trace, "\tPhysical Memory (Available/Total): {0} / {1}",
			Utility::BytesToString(AvailablePhysMemory),
			Utility::BytesToString(TotalPhysMemory));

		LOG(Core, Trace, "\tVirtual Memory  (Available/Total): {0} / {1}",
			Utility::BytesToString(AvailableVirtualMemory),
			Utility::BytesToString(TotalVirtualMemory));

		LOG(Core, Trace, "-------------------------------------------------------------------------");
	}

	std::optional<SystemMemoryInfo> System::GetMemoryInfo(void)
	{
	#ifdef PLATFORM_WINDOWS
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&status);

		SystemMemoryInfo result =
		{
			static_cast<uint64_t>(status.ullAvailPhys),
			static_cast<uint64_t>(status.ullTotalPhys),
			static_cast<uint64_t>(status.ullAvailVirtual),
			static_cast<uint64_t>(status.ullTotalVirtual)
		};
		return result;
	#else
		LOG(Core, Warning, "SystemMemoryInfo is not supported for this platform.");
		return std::nullopt;
	#endif
	}

	std::optional<SystemVideoMemoryInfo> System::GetVideoMemoryInfo(void)
	{
		LOG(Core, Warning, "SystemVideoMemoryInfo is not supported for this platform.");
		return std::nullopt;
	}

	MemoryStatistics const& System::GetMemoryStatistics(void)
	{
		return g_MemoryManager.GetMemoryStats();
	}
}