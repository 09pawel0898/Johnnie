#include "EnginePCH.hpp"

#include "System.hpp"

#include "Log/Log.hpp"
#include "Utilities/StringUtility.hpp"

#include "Core/PlatformCheck.hpp"

/** TO BE REMoveTempD (no explicit glad includes despite RHI) */
#include <glad/glad.h>
#include "Renderer/RHI/Platform/OpenGL/Extensions/OpenGLExtensionCheck.hpp"
//

#ifdef PLATFORM_WINDOWS
#	include <Windows.h>
#endif

namespace Engine
{
	System::GetVideoMemoryInfoFunc System::s_GetVideoMemoryInfoFunc = nullptr;

	void System::Init()
	{
		InitGraphicsCardBrandingInfo();
		InitVideoMemoryInfo();
		s_bInitialized = true;
	}

	static SystemVideoMemoryInfo GetNvidiaVideoMemoryInfo()
	{
		SystemNvidiaVideoMemoryInfo NvidiaMemoryInfo;

		GLint kb;

		glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &kb);
		NvidiaMemoryInfo.DedicatedVideoMemory = kb;
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &kb);
		NvidiaMemoryInfo.TotalAvailableVideoMemory = kb;
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &kb);
		NvidiaMemoryInfo.CurrentAvailableVideoMemory = kb;

		glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &kb);
		NvidiaMemoryInfo.EvictedCount = kb;
		glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &kb);
		NvidiaMemoryInfo.EvictedSize = kb;
		
		return SystemVideoMemoryInfo(NvidiaMemoryInfo);
	}

	static SystemVideoMemoryInfo GetAMDVideoMemoryInfo()
	{
		SystemAMDVideoMemoryInfo AMDMemoryInfo;

		GLint kb[4];

		glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, kb);
		AMDMemoryInfo.TotalPool		= kb[0];
		AMDMemoryInfo.LargestBlock	= kb[1];
		AMDMemoryInfo.TotalAux		= kb[2];
		AMDMemoryInfo.LargestAux	= kb[3];

		return SystemVideoMemoryInfo(AMDMemoryInfo);
	}

	static SystemVideoMemoryInfo GetMemoryInfo_NotSupported()
	{
		LOG(Core, Warning, "SystemVideoMemoryInfo is not supported for your configuration.");
		return SystemVideoMemoryInfo(nullptr);
	}

	void System::InitVideoMemoryInfo(void)
	{
		using namespace Utility::OpenGL;
		
		if (IsExtensionSupported("GL_NVX_gpu_memory_info"))
		{
			s_GetVideoMemoryInfoFunc = GetNvidiaVideoMemoryInfo;
		}
		else if (IsExtensionSupported("GL_ATI_meminfo"))
		{
			s_GetVideoMemoryInfoFunc = GetAMDVideoMemoryInfo;
		}
		else
		{
			s_GetVideoMemoryInfoFunc = GetMemoryInfo_NotSupported;
		}
	}

	void System::Shutdown(void)
	{}

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

	SystemVideoMemoryInfo System::GetVideoMemoryInfo(void)
	{
		if (!s_bInitialized)
		{
			LOG(Core, Warning, "System :: GetVideoMemoryInfo - System not initialized yet.");
			return SystemVideoMemoryInfo(nullptr);
		}
		return s_GetVideoMemoryInfoFunc();
	}

	MemoryStatistics const& System::GetMemoryStatistics(void)
	{
		return g_MemoryManager.GetMemoryStats();
	}

	SystemVideoBrandingInfo System::GetGraphicsCardBrandingInfo(void)
	{
		if (!s_bInitialized)
		{
			LOG(Core, Warning, "System :: GetGraphicsCardBrandingInfo - System not initialized yet.");
		}
		return s_BrandingInfo;
	}

	void System::InitGraphicsCardBrandingInfo(void)
	{
		s_BrandingInfo.Vendor	= reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		s_BrandingInfo.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

		if (s_BrandingInfo.Vendor == "NVIDIA Corporation")
		{
			s_BrandingInfo.Brand = GpuBrand::NVIDIA;
		}
		else if (s_BrandingInfo.Vendor == "ATI Technologies Inc.")
		{
			s_BrandingInfo.Brand = GpuBrand::AMD;
		}
	}

	void SystemMemoryInfo::LogManager(void) const
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

	void SystemVideoBrandingInfo::LogManager(void) const
	{
		LOG(Core, Trace, "------------------------------Graphics Card------------------------------");

		LOG(Core, Trace, "\tVendor : {0}"	, Vendor);
		LOG(Core, Trace, "\tRenderer : {0} ", Renderer);

		LOG(Core, Trace, "-------------------------------------------------------------------------");
	}

	void SystemNvidiaVideoMemoryInfo::LogManager(void) const
	{
		LOG(Core, Trace, "------------------------------Video Memory-------------------------------");

		LOG(Core, Trace, "\tDedicated video memory : {0}",			Utility::BytesToString(1024*DedicatedVideoMemory));
		LOG(Core, Trace, "\tTotal available video memory : {0}",	Utility::BytesToString(1024 * TotalAvailableVideoMemory));
		LOG(Core, Trace, "\tCurrent available video memory : {0}",	Utility::BytesToString(1024 * CurrentAvailableVideoMemory));
		LOG(Core, Trace, "\tEvicted memory count  : {0}",			EvictedCount);
		LOG(Core, Trace, "\tEvicted memory size : {0}",				Utility::BytesToString(1024 * EvictedSize));

		LOG(Core, Trace, "-------------------------------------------------------------------------");
	}

	void SystemAMDVideoMemoryInfo::LogManager(void) const
	{
		LOG(Core, Trace, "------------------------------Video Memory-------------------------------");

		LOG(Core, Trace, "\tTotal memory free in the pool : {0}",				Utility::BytesToString(1024 * TotalPool));
		LOG(Core, Trace, "\tLargest available free block in the pool : {0}",	Utility::BytesToString(1024 * LargestBlock));
		LOG(Core, Trace, "\tTotal auxiliary memory free : {0}",					Utility::BytesToString(1024 * TotalAux));
		LOG(Core, Trace, "\tLargest auxiliary free block  : {0}",				Utility::BytesToString(1024 * LargestAux));

		LOG(Core, Trace, "-------------------------------------------------------------------------");
	}
}