#include "EnginePCH.hpp"
#include "EngineBaseLayer.hpp"

#include "Renderer/Camera/CameraController.hpp"

namespace Engine
{
	EngineBaseLayer::EngineBaseLayer(std::string_view Name) noexcept
		:	Layer(Name)
	{
	}

	void EngineBaseLayer::OnAwake(void)
	{
	}

	void EngineBaseLayer::OnDetach(void)
	{
	}

	void EngineBaseLayer::OnTick(double DeltaTime)
	{
		CameraController::Get()->OnTick(DeltaTime);
	}

	void EngineBaseLayer::OnEvent(Events::Event& Event)
	{
		CameraController::Get()->OnEvent(Event);
	}

	void EngineBaseLayer::OnRender(void) const
	{
	}

	void EngineBaseLayer::OnRenderGui(void)
	{
	}
}