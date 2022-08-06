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
		virtual void PostInit(void) override {}
		virtual void Shutdown(void) override {}
	};
}