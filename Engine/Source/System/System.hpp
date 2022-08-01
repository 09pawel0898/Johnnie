#pragma once

#include <optional>
#include <variant>
#include <iostream>
#include <functional>

#include "MemoryManager.hpp"

namespace Engine
{
	enum
	{
		None = -1
	};

	enum class GpuBrand : uint8_t
	{
		UNKNOWN, NVIDIA, AMD
	};

	struct SystemInfo
	{
		virtual void Log(void) const = 0;
	};

	struct SystemMemoryInfo final : public SystemInfo
	{
		uint64_t AvailablePhysMemory;
		uint64_t TotalPhysMemory;

		uint64_t AvailableVirtualMemory;
		uint64_t TotalVirtualMemory;

		SystemMemoryInfo(uint64_t AvailablePhysMemory, uint64_t TotalPhysMemory, uint64_t AvailableVirtualMemory, uint64_t TotalVirtualMemory)
			:	AvailablePhysMemory(AvailablePhysMemory), TotalPhysMemory(TotalPhysMemory), AvailableVirtualMemory(AvailableVirtualMemory), TotalVirtualMemory(TotalVirtualMemory)
		{}

		void Log(void) const override;
	};
	
	struct SystemVideoBrandingInfo final : public SystemInfo
	{
		GpuBrand Brand = GpuBrand::UNKNOWN;

		std::string Vendor	= "none";
		std::string Renderer= "none";

		void Log(void) const override;
	};

	/** Parsed from GL_NVX_gpu_memory_info */
	struct SystemNvidiaVideoMemoryInfo final : public SystemInfo
	{
		int64_t DedicatedVideoMemory		= None;	// Dedicated video memory, total size (in kb) of the GPU memory
		int64_t TotalAvailableVideoMemory	= None;	// Total available memory, total size (in Kb) of the memory available for allocations
		int64_t CurrentAvailableVideoMemory = None; // Current available dedicated video memory (in kb), currently unused GPU memory

		int64_t EvictedCount	= None;				// Count of total evictions seen by system
		int64_t EvictedSize		= None;				// Size of total video memory evicted (in kb)

		void Log(void) const override;
	};

	/** Parsed from GL_ATI_meminfo */
	struct SystemAMDVideoMemoryInfo final : public SystemInfo
	{
		int64_t TotalPool		= None;				// Total memory free in the pool
		int64_t LargestBlock	= None;				// Largest available free block in the pool
		int64_t TotalAux		= None;				// Total auxiliary memory free
		int64_t LargestAux		= None;				// Largest auxiliary free block

		void Log(void) const override;
	};

	struct SystemVideoMemoryInfo
	{
	private:
		std::variant<std::nullptr_t,
					SystemNvidiaVideoMemoryInfo,
					SystemAMDVideoMemoryInfo> Info;
	public:

		template <typename BrandVideoMemoryInfoType>
		explicit SystemVideoMemoryInfo(BrandVideoMemoryInfoType&& VideoMemoryInfo) noexcept
			:	Info(std::forward<BrandVideoMemoryInfoType>(VideoMemoryInfo))
		{}

		template<typename BrandVideoMemoryInfoType>
		std::optional<BrandVideoMemoryInfoType> Get(void)
		{
			BrandVideoMemoryInfoType* resultInfo = std::get_if<BrandVideoMemoryInfoType>(&Info);
			if (resultInfo)
			{
				return *resultInfo;
			}
			else
			{
				return std::nullopt;
			}
		}
	};

	class System
	{
	private:
		static inline bool s_bInitialized = false;
		static inline SystemVideoBrandingInfo s_BrandingInfo;

		using GetVideoMemoryInfoFunc = std::function<SystemVideoMemoryInfo(void)>;

		static GetVideoMemoryInfoFunc s_GetVideoMemoryInfoFunc;

	public:
		static void Init(void);
		static void Shutdown(void);

	private:
		static void InitGraphicsCardBrandingInfo(void);
		static void InitVideoMemoryInfo(void);

	public:		
		/** Physical and virtual memory info */
		static std::optional<SystemMemoryInfo> GetMemoryInfo(void);

		/** Video memory info */
		static SystemVideoMemoryInfo GetVideoMemoryInfo(void);

		/** Video memory branding info */
		static SystemVideoBrandingInfo GetGraphicsCardBrandingInfo(void);

		/** Allocated memory info */
		static MemoryStatistics const& GetMemoryStatistics(void);
	};
}