#pragma once

#include "RHI/DynamicRHI.hpp"

namespace Engine
{
	using namespace RHI;

	class Renderer final
	{
	public:
		static void Init(RenderingAPI RenderingAPI);

		static void Shutdown(void);
	
	private:
		static std::unique_ptr<DynamicRHI> s_RHI;

	public:
		static std::unique_ptr<DynamicRHI>& GetRHI(void);
		static RenderingAPI GetApiType(void);
	};
}