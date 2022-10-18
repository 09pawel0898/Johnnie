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

	namespace Utility
	{
		static FORCEINLINE auto FindFramebufferByName(std::string_view FramebufferName, std::vector<std::pair<std::string_view, std::unique_ptr<RHIFrameBuffer>>>& FramebuffersContainer)
		{
			return std::find_if(FramebuffersContainer.begin(), FramebuffersContainer.end(),
				[FramebufferName](std::pair<std::string_view, std::unique_ptr<RHIFrameBuffer>> const& Pair)
				{
					return Pair.first == FramebufferName;
				});
		}
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

	void DynamicRHI::InitializeFramebuffer(std::string_view FramebufferName, RHIFrameBufferSpecification const& FramebufferSpecification)
	{
		if (m_Framebuffers.end() == Utility::FindFramebufferByName(FramebufferName,m_Framebuffers))
		{
			std::unique_ptr<RHIFrameBuffer> newFrameBuffer = RHIFrameBuffer::Create(FramebufferSpecification);
			m_Framebuffers.emplace_back(std::make_pair(FramebufferName,std::move(newFrameBuffer)));
		}
		else
		{
			CheckMsg(false, "Framebuffer of name provided already exists. Failed to create.");
		}
	}

	std::unique_ptr<RHIFrameBuffer>& DynamicRHI::GetFramebuffer(std::string_view FramebufferName)
	{
		auto foundFramebuffer = Utility::FindFramebufferByName(FramebufferName, m_Framebuffers);
	
		if (foundFramebuffer == m_Framebuffers.end())
		{
			CheckMsg(false, "Requested framebuffer was not initialized.");
		}
		return foundFramebuffer->second;
	}

	void DynamicRHI::BindFramebuffer(std::string_view FramebufferName)
	{
		auto foundFramebuffer = Utility::FindFramebufferByName(FramebufferName, m_Framebuffers);

		if (foundFramebuffer == m_Framebuffers.end())
		{
			CheckMsg(false, "Bind failed. Requested framebuffer was not initialized.");
		}
		foundFramebuffer->second->Bind();
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