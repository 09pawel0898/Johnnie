#include "System.hpp"

#include "Utilities/Log.hpp"

#include "Core/PlatformCheck.hpp"

#ifdef PLATFORM_WINDOWS
#	include <Windows.h>
#endif

#include <sstream>
#include <iomanip>
#include <bit>

namespace Engine
{
	static std::string BytesToString(uint64_t Bytes)
	{
		static constexpr float GB = 1024 * 1024 * 1024;
		static constexpr float MB = 1024 * 1024;
		static constexpr float KB = 1024;

		std::ostringstream result;

		auto GetVerbosity = [](auto const& Bytes) -> std::pair<float, std::string_view>
		{
			if (Bytes > GB)
			{
				return std::make_pair((Bytes / GB), " GB");
			}
			else if (Bytes > MB)
			{
				return std::make_pair((Bytes / MB), " MB");
			}
			else if (Bytes > KB)
			{
				return std::make_pair((Bytes / KB), " KB");
			}
			else
			{
				return std::make_pair(static_cast<float>(Bytes), " bytes");
			}
		};

		auto [number, postfix] = GetVerbosity(Bytes);
		result << std::fixed << std::setprecision(2) << number << postfix;

		return result.str();
	}

	void SystemMemoryInfo::Log() const
	{
		LOG(Core, Trace, "Memory Info:");

		LOG(Core, Trace, "\tPhysical Memory (Available/Total): {0} / {1}",
			BytesToString(AvailablePhysMemory),
			BytesToString(TotalPhysMemory));

		LOG(Core, Trace, "\tVirtual Memory  (Available/Total): {0} / {1}",
			BytesToString(AvailableVirtualMemory),
			BytesToString(TotalVirtualMemory));
	}

	std::optional<SystemMemoryInfo> System::GetMemoryInfo()
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

	std::optional<SystemVideoMemoryInfo> System::GetVideoMemoryInfo()
	{
		LOG(Core, Warning, "SystemVideoMemoryInfo is not supported for this platform.");
		return std::nullopt;
	}

	MemoryStatistics const& System::GetMemoryStatistics()
	{
		return MemoryManager::Get()->GetMemoryStats();
	}
}