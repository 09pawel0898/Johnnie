#pragma once

#include "../../DynamicRHI.hpp"

namespace Engine::RHI
{
	class OpenGLRHI final : public DynamicRHI
	{
	public:
		OpenGLRHI() : DynamicRHI(RenderingAPI::OpenGL)
		{}

	public:
		static constexpr uint8_t OpenGL_MAJOR_VERSION = 4;
		static constexpr uint8_t OpenGL_MINOR_VERSION = 6;

		virtual void Init(void) override;
		virtual void Shutdown(void) override {}

		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) override;
		virtual void SetClearColor(const glm::vec4& color)override;
		virtual void Clear(void) override;
	};
}