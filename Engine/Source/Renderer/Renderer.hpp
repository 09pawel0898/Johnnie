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

	class Renderer final : public Singleton<Renderer>
	{
	public:
		static void Init(RenderingAPI RenderingAPI);

		static void Shutdown(void);
	
	public:
		std::unique_ptr<DynamicRHI>& GetRHI(void);
		RenderingAPI GetApiType(void);

	private:
		std::shared_ptr<OCamera> m_Camera = nullptr;
	public:

		void SetViewTarget(std::shared_ptr<OCamera> ViewTarget);

		void Draw(std::shared_ptr<RHIShader> const& Shader, std::shared_ptr<RHIVertexArray> const& VertexArray, glm::mat4 const& ModelMat);
	};
}