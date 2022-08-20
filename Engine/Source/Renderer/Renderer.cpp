#include "EnginePCH.hpp"

#include "Renderer.hpp"

namespace Engine
{
	void Renderer::Init(RenderingAPI RenderingAPI)
	{
		Construct();

		auto& _this = Get();

		if (_this->m_RHI = DynamicRHI::Create(RenderingAPI))
		{
			_this->m_RHI->Init();
		}
		else
		{
			Check(false);
		}
	}
	
	void Renderer::Shutdown(void)
	{
		auto& _this = Get();

		_this->m_RHI->Shutdown();
	}
	
	std::unique_ptr<DynamicRHI>& Renderer::GetRHI(void)
	{
		return m_RHI;
	}

	RenderingAPI Renderer::GetApiType(void)
	{
		return m_RHI->GetType();
	}

	void Renderer::SetRenderTarget(std::shared_ptr<Camera> RenderTarget)
	{
		m_Camera = std::move(RenderTarget);
	}
}