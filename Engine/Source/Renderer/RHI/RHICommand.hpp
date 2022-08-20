#pragma once

#include "Core/CoreMinimal.hpp"
#include "DynamicRHI.hpp"

namespace Engine::RHI
{
	class RHICommand
	{
	private:
		static std::unique_ptr<DynamicRHI> s_DynamicRHI;

		friend class Renderer;

	private:
		static std::unique_ptr<DynamicRHI>& GetRHI(void)
		{
			return s_DynamicRHI;
		}

		static void Initialize(RenderingAPI RenderingAPI);

		static void Shutdown(void);

		/** RHI Methods */

		static void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height);

		static void SetClearColor(const glm::vec4& Color);

		static void Clear(void);

		static void DrawLines(std::shared_ptr<RHIVertexArray> const& VertexArray, uint32_t VertexCount = 0);

		static void DrawIndexed(std::shared_ptr<RHIVertexArray> const& VertexArray, uint32_t IndexCount = 0);
	};

	FORCEINLINE void RHICommand::Initialize(RenderingAPI RenderingAPI)
	{
		s_DynamicRHI = DynamicRHI::Create(RenderingAPI);
		Check(s_DynamicRHI);

		s_DynamicRHI->Init();
	}

	FORCEINLINE void RHICommand::Shutdown(void)
	{
		s_DynamicRHI->Shutdown();
	}

	FORCEINLINE void RHICommand::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
	{
		s_DynamicRHI->SetViewport(X, Y, Width, Height);
	}

	FORCEINLINE void RHICommand::SetClearColor(const glm::vec4& Color)
	{
		s_DynamicRHI->SetClearColor(Color);
	}

	FORCEINLINE void RHICommand::Clear(void)
	{
		s_DynamicRHI->Clear();
	}

	FORCEINLINE void RHICommand::DrawLines(std::shared_ptr<RHIVertexArray> const& VertexArray, uint32_t VertexCount)
	{
		s_DynamicRHI->DrawLines(VertexArray, VertexCount);
	}

	FORCEINLINE void RHICommand::DrawIndexed(std::shared_ptr<RHIVertexArray> const& VertexArray, uint32_t IndexCount)
	{
		s_DynamicRHI->DrawIndexed(VertexArray, IndexCount);
	}
}