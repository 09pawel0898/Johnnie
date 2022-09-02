#include "EnginePCH.hpp"
#include "EngineBaseLayer.hpp"

#include "Renderer/Camera/CameraController.hpp"
#include "Scene/Entities/Primitives/Tickable.hpp"

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
		/** Tick camera controller */
		CameraController::Get()->OnTick(DeltaTime);

		/** Tick tickable entities  */
		TickableManager::Get()->UdateTickableEntities(DeltaTime);
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