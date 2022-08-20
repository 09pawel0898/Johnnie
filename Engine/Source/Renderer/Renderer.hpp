#pragma once

#include "RHI/DynamicRHI.hpp"

#include "Utilities/Singleton.hpp"

namespace Engine
{
	class Camera;

	using namespace RHI;

	class Renderer final : public Singleton<Renderer>
	{
	public:
		static void Init(RenderingAPI RenderingAPI);

		static void Shutdown(void);
	
	private:
		std::unique_ptr<DynamicRHI> m_RHI = nullptr;

	public:
		std::unique_ptr<DynamicRHI>& GetRHI(void);
		RenderingAPI GetApiType(void);

	private:
		std::shared_ptr<Camera> m_Camera = nullptr;
	public:

		void SetRenderTarget(std::shared_ptr<Camera> RenderTarget);
	};
}