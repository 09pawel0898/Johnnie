#pragma once

#include "Core/CoreMinimal.hpp"
#include "Resources/RHIVertexArray.hpp"
#include "Resources/RHIResourceManager.hpp"
#include "Resources/RHIFrameBuffer.hpp"

#include "Utilities/FlagOperators.hpp"

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

	enum RHIRenderingFlags
	{
		R_Wireframe			= 1 << 0,
		R_BoneInfuence		= 1 << 1,
		R_ShadowMap			= 1 << 3
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
		static TUniquePtr<DynamicRHI> Create(RenderingAPI RenderingAPI);

	protected:
		RenderingAPI		m_RHIType;
		RHIRenderingFlags	m_RenderingFlags;
		
	public:
		RenderingAPI GetType(void) const
		{
			return m_RHIType;
		}

		virtual void Init(void) = 0;
		virtual void Shutdown(void) = 0;

	private:
		/** RHI Resources */

		RHITexture2DManager m_Texture2DManager;
		RHIShaderManager	m_ShaderManager;
		MaterialManager		m_MaterialManager;

		std::vector<std::pair<std::string_view,TUniquePtr<RHIFrameBuffer>>> m_Framebuffers;
	
	public:
		RHITexture2DManager&	GetTexture2DManager(void);
		RHIShaderManager&		GetShaderManager(void);
		MaterialManager&		GetMaterialManager(void);

		/** RHI Methods */

		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) = 0;
		virtual void SetClearColor(const glm::vec4& Color) = 0;
		virtual void Clear(void) = 0;
		
		virtual void DrawLines(TSharedPtr<RHIVertexArray> const& VertexArray, uint32_t VertexCount = 0) = 0;
		virtual void DrawIndexed(TSharedPtr<RHIVertexArray> const& VertexArray, uint32_t IndexCount = 0) = 0;
		
		void InitializeFramebuffer(std::string_view FramebufferName, RHIFrameBufferSpecification const& FramebufferSpecification);
		TUniquePtr<RHIFrameBuffer>& GetFramebuffer(std::string_view FramebufferName);
		void BindFramebuffer(std::string_view FramebufferName);
		virtual void BindDefaultFramebuffer(void) = 0;

	public:
		/** Shaders / Materials binding management */
		static constexpr int8_t s_MaxTexturesPerShader = 8;

		AUUID m_BoundShaderUUID		= ID::None;
		AUUID m_BoundMaterialUUID	= ID::None;

		void SetBoundShaderUUID(AUUID const& UUID);
		AUUID GetBoundShaderUUID(void) const;

		void SetBoundMaterialUUID(AUUID const& UUID);
		AUUID GetBoundMaterialUUID(void) const;

	public:
		void SetRenderingFlags(RHIRenderingFlags Flags);
		void SetRenderingFlag(RHIRenderingFlags Flag);
		void ClearRenderingFlag(RHIRenderingFlags Flag);
		RHIRenderingFlags GetRenderingFlags(void) const;
		bool HasFlag(RHIRenderingFlags Flag) const;

	protected:
		virtual void OnRenderingFlagsUpdated(void) = 0;

		virtual void SetWireframeMode(bool Enable) = 0;
	};

	FORCEINLINE RHITexture2DManager& DynamicRHI::GetTexture2DManager(void)
	{
		return m_Texture2DManager;
	}

	FORCEINLINE RHIShaderManager& DynamicRHI::GetShaderManager(void)
	{
		return m_ShaderManager;
	}
	
	FORCEINLINE MaterialManager& DynamicRHI::GetMaterialManager(void)
	{
		return m_MaterialManager;
	}

	FORCEINLINE void DynamicRHI::SetRenderingFlags(RHIRenderingFlags Flags)
	{
		m_RenderingFlags = Flags;
		OnRenderingFlagsUpdated();
	}

	FORCEINLINE void DynamicRHI::SetRenderingFlag(RHIRenderingFlags Flag)
	{
		m_RenderingFlags |= Flag;
		OnRenderingFlagsUpdated();
	}

	FORCEINLINE void DynamicRHI::ClearRenderingFlag(RHIRenderingFlags Flag)
	{
		m_RenderingFlags &= ~Flag;
		OnRenderingFlagsUpdated();
	}

	FORCEINLINE RHIRenderingFlags DynamicRHI::GetRenderingFlags(void) const
	{
		return m_RenderingFlags;
	}
	
	FORCEINLINE bool DynamicRHI::HasFlag(RHIRenderingFlags Flag) const
	{
		return Flag & m_RenderingFlags;
	}
}