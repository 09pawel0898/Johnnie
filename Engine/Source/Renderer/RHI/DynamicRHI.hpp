#pragma once

#include "Core/CoreMinimal.hpp"
#include "Resources/RHIVertexArray.hpp"

#include <glm/glm.hpp>

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

		RenderingAPI GetType(void) const
		{
			return m_RHIType;
		}

		virtual void Init(void) = 0;
		virtual void Shutdown(void) = 0;

		/** RHI Methods */

		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) = 0;
		virtual void SetClearColor(const glm::vec4& Color) = 0;
		virtual void Clear(void) = 0;

		virtual void DrawLines(std::shared_ptr<RHIVertexArray> const& VertexArray, uint32_t VertexCount = 0) = 0;
		virtual void DrawIndexed(std::shared_ptr<RHIVertexArray> const& VertexArray, uint32_t IndexCount = 0) = 0;
	};
}