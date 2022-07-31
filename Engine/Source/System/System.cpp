#include "System.hpp"

#include "Utilities/Log.hpp"
#include "Utilities/StringUtility.hpp"
#include "Utilities/OpenGL/OpenGLExtensionCheck.hpp"

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
			Utilities::BytesToString(AvailablePhysMemory),
			Utilities::BytesToString(TotalPhysMemory));

		LOG(Core, Trace, "\tVirtual Memory  (Available/Total): {0} / {1}",
			Utilities::BytesToString(AvailableVirtualMemory),
			Utilities::BytesToString(TotalVirtualMemory));

		LOG(Core, Trace, "-------------------------------------------------------------------------");
	}

	void System::Init()
	{
		InitVideoMemoryInfo();
		s_bInitialized = true;
	}

	void System::InitVideoMemoryInfo(void)
	{
		//limits_func = limits_not_supported;
		//evict_func = eviction_not_supported;

		//std::vector<std::string> exts;
		
		using namespace Utilities::OpenGL;
		
		if (IsExtensionSupported("GL_NVX_gpu_memory_info"))
		{
			LOG(Core, Trace, "GL_NVX_gpu_memory_info supported.");
		}
		else if (IsExtensionSupported("GL_ATI_meminfo"))
		{
			LOG(Core, Trace, "GL_ATI_meminfo supported.");
		}
		/*
		std::vector<bool> supported = ogle::extensions_supported(exts);

		if (supported[0]) {
			limits_func = limits_nvidia;
			evict_func = eviction_nvidia;
		}
		else if (supported[1]) {
			limits_func = limits_ati;
		}*/
	}

	void System::Shutdown(void)
	{
	}

	std::optional<SystemMemoryInfo> System::GetMemoryInfo(void)
	{
		if (!s_bInitialized)
		{
			LOG(Core, Warning, "System :: GetMemoryInfo - System not initialized yet.");
			return std::nullopt;
		}

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
		if (!s_bInitialized)
		{
			LOG(Core, Warning, "System :: GetVideoMemoryInfo - System not initialized yet.");
			return std::nullopt;
		}

		LOG(Core, Warning, "SystemVideoMemoryInfo is not supported for this platform.");
		return std::nullopt;
	}

	MemoryStatistics const& System::GetMemoryStatistics(void)
	{
		return g_MemoryManager.GetMemoryStats();
	}
}