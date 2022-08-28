#pragma once

#include "RHI/DynamicRHI.hpp"

#include "Utilities/Singleton.hpp"

namespace Engine
{
	class OCamera;

	namespace RHI
	{
		class RHIShader;
	}
	using namespace RHI;

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
	
	public:
		/** Resource Management */

		RHIShaderManager& GetShaderManager(void) const;
		RHITexture2DManager& GetTexture2DManager(void) const;
	};
}