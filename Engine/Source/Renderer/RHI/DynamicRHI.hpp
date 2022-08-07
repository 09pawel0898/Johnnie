#pragma once

#include "Core/CoreMinimal.hpp"

namespace Engine::RHI
{
	enum class RenderingAPI : uint8_t
	{
		OpenGL = 0,
		D3D11,
		D3D12,
		Vulkan
	};

	class DynamicRHI
	{
	protected:
		DynamicRHI(RenderingAPI RenderingAPI);

		DynamicRHI(DynamicRHI const&) = delete;
		DynamicRHI(DynamicRHI&&) = delete;

		DynamicRHI& operator=(DynamicRHI&&) = delete;
		DynamicRHI operator&(DynamicRHI const&) = delete;

	public:
		virtual ~DynamicRHI() = default;

		[[nodiscard]]
		static std::unique_ptr<DynamicRHI> Create(RenderingAPI RenderingAPI);

	public:
		RenderingAPI m_RHIType;

		FORCEINLINE RenderingAPI GetType(void) const
		{
			return m_RHIType;
		}

		virtual void Init(void) = 0;

		virtual void Shutdown(void) = 0;
	};
}