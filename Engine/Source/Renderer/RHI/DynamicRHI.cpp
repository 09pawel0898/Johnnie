#include "EnginePCH.hpp"

#include "DynamicRHI.hpp"
#include "Log/Log.hpp"

#include "Platform/OpenGL/OpenGLRHI.hpp"

namespace Engine::RHI
{
	DynamicRHI::DynamicRHI(RenderingAPI RenderingAPI)
		:	m_RHIType(RenderingAPI)
	{
		m_BoundTexturesUUID.fill(ID::None);
	}

	std::unique_ptr<DynamicRHI> DynamicRHI::Create(RenderingAPI RenderingAPI)
	{
		switch (RenderingAPI)
		{
			case RenderingAPI::OpenGL:
				return std::make_unique<OpenGLRHI>(); 
			break;
			case RenderingAPI::D3D11:	
				LOG(RHI, Error, "D3D11RHI is not supported.");
				return nullptr; 
				break;
			case RenderingAPI::D3D12:	
				LOG(RHI, Error, "D3D12RHI is not supported.");
				return nullptr; 
				break;
			case RenderingAPI::Vulkan:	
				LOG(RHI, Error, "VulkanRHI is not supported.");
				return nullptr; 
				break;
		}
		return nullptr;
	}

	AUUID DynamicRHI::GetBoundShaderUUID(void) const
	{
		return m_BoundShaderUUID;
	}

	AUUID DynamicRHI::GetBoundMaterialUUID(void) const
	{
		return m_BoundMaterialUUID;
	}

	void DynamicRHI::SetBoundShaderUUID(AUUID const& UUID)
	{
		m_BoundShaderUUID = UUID;
	}

	void DynamicRHI::SetBoundMaterialUUID(AUUID const& UUID)
	{
		m_BoundMaterialUUID = UUID;
	}
}