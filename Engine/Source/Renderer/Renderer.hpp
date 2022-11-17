#pragma once

#include "RHI/DynamicRHI.hpp"
#include "RHI/Resources/RHIFrameBuffer.hpp"

#include "Utilities/Singleton.hpp"
#include "RendererStatistics.hpp"
#include "Core/Application/Application.hpp"
#include "Utilities/FlagOperators.hpp"

namespace Engine
{
	class ACamera;

	namespace RHI
	{
		class RHIShader;
	}

	using namespace RHI;
	using namespace Core;

	enum RenderingFlags
	{
		R_None			= 0,
		R_Wireframe		= 1 << 0,
		R_BoneInfuence	= 1 << 1,
		R_ShadowMap		= 1 << 2
	};

	struct ShadowsSettings
	{
	public:
		bool	bRenderShadows{ true };
		float	ShadowsIntensity = 0.55f;
	};

	class Renderer final : public MSingleton<Renderer>
	{
	private:
		RenderingFlags	m_RenderingFlags = R_None;
		ShadowsSettings	m_ShadowsSettings;

	public:
		static void Init(RenderingAPI RenderingAPI);

		static void Shutdown(void);
	
	public:
		TUniquePtr<DynamicRHI>& GetRHI(void);
		RenderingAPI GetApiType(void);

	public:
		void InitializeViewport(glm::i32vec4 ViewportBounds);

		void InitializeFramebuffer(std::string_view FramebufferName, RHIFrameBufferSpecification const& FramebufferSpecification);
		TUniquePtr<RHIFrameBuffer>& GetFramebuffer(std::string_view FramebufferName);

		void BindFramebuffer(std::string_view FramebufferName);
		void BindDefaultFramebuffer(void);

		void Clear(void);
		void Draw(TSharedPtr<RHIShader> const& Shader, TSharedPtr<RHIVertexArray> const& VertexArray, glm::mat4 const& ModelMat);
		
		void OnBeginRenderingFrame(void);
		void OnEndRenderingFrame(void);

		void OnRenderingFlagsUpdated(RenderingFlags Flags);

	public:
		/** Resource Management */

		RHIShaderManager&		GetShaderManager(void) const;
		RHITexture2DManager&	GetTexture2DManager(void) const;
		MaterialManager&		GetMaterialManager(void) const;


		/** Stats */
	private:
		static inline RendererStatistics s_RendererStats;
		
		friend class Core::Application;

	public:
		static RendererStatistics const& GetRendererStats(void);
		static RendererStatistics& GetMutableRendererStats(void);


	public:
		void SetRenderingFlags(RenderingFlags Flags);

		RenderingFlags GetRenderingFlags(void) const;

		void SetRenderingFlag(RenderingFlags Flag);
		void ClearRenderingFlag(RenderingFlags Flag);
		bool HasFlag(RenderingFlags Flag) const;

	public:
		ShadowsSettings& GetShadowsSettings(void);
	};

	FORCEINLINE void Renderer::SetRenderingFlags(RenderingFlags Flags)
	{
		m_RenderingFlags = Flags;
		OnRenderingFlagsUpdated(Flags);
	}

	FORCEINLINE void Renderer::SetRenderingFlag(RenderingFlags Flag)
	{
		m_RenderingFlags |= Flag;
		OnRenderingFlagsUpdated(m_RenderingFlags);
	}

	FORCEINLINE void Renderer::ClearRenderingFlag(RenderingFlags Flag)
	{
		m_RenderingFlags &= ~Flag;
		OnRenderingFlagsUpdated(m_RenderingFlags);
	}

	FORCEINLINE RenderingFlags Renderer::GetRenderingFlags(void) const
	{
		return m_RenderingFlags;
	}

	FORCEINLINE bool Renderer::HasFlag(RenderingFlags Flag) const
	{
		return Flag & m_RenderingFlags;
	}

	FORCEINLINE ShadowsSettings& Renderer::GetShadowsSettings(void)
	{
		return m_ShadowsSettings;
	}
}