#pragma once

#include "RHI/DynamicRHI.hpp"

#include "Utilities/Singleton.hpp"
#include "RendererStatistics.hpp"
#include "Core/Application/Application.hpp"

namespace Engine
{
	class ACamera;

	namespace RHI
	{
		class RHIShader;
	}
	using namespace RHI;
	using namespace Core;

	class Renderer final : public MSingleton<Renderer>
	{
	public:
		static void Init(RenderingAPI RenderingAPI);

		static void Shutdown(void);
	
	public:
		std::unique_ptr<DynamicRHI>& GetRHI(void);
		RenderingAPI GetApiType(void);

	public:
		void InitializeViewport(glm::i32vec4 ViewportBounds);

		void Clear(void);

		void Draw(std::shared_ptr<RHIShader> const& Shader, std::shared_ptr<RHIVertexArray> const& VertexArray, glm::mat4 const& ModelMat);
		
		void OnBeginRenderingFrame(void);
		void OnEndRenderingFrame(void);
	public:
		/** Resource Management */

		RHIShaderManager&		GetShaderManager(void) const;
		RHITexture2DManager&	GetTexture2DManager(void) const;
		MaterialManager&		GetMaterialManager(void) const;


		/** Stats */
	private:
		static inline RendererStatistics s_RendererStats;
		
		static void UpdateRendererStats(void);
		friend class Core::Application;

	public:
		static RendererStatistics const& GetRendererStats(void);
		static RendererStatistics& GetMutableRendererStats(void);
	};
}