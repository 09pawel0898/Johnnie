#include "EnginePCH.hpp"

#include "Renderer.hpp"

namespace Engine
{
	std::unique_ptr<DynamicRHI> Renderer::s_RHI = nullptr;

	void Renderer::Init(RenderingAPI RenderingAPI)
	{
		if (s_RHI = DynamicRHI::Create(RenderingAPI))
		{
			s_RHI->Init();
		}
		else
		{
			Check(false);
		}
	}
	
	void Renderer::Shutdown(void)
	{
		s_RHI->Shutdown();
	}
	
	std::unique_ptr<DynamicRHI>& Renderer::GetRHI(void)
	{
		return s_RHI;
	}

	RenderingAPI Renderer::GetApiType(void)
	{
		return s_RHI->GetType();
	}
}