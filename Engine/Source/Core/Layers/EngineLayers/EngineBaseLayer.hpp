#pragma once

#include "../Layer.hpp"

namespace Engine
{
	class EngineBaseLayer final : public Engine::Layer
	{
	public:
		explicit EngineBaseLayer(std::string_view Name) noexcept;

	private:
		/** Layer Interface Impl*/
		virtual void OnAwake(void) override;
		virtual void OnDetach(void) override;

		virtual void OnTick(double DeltaTime) override;
		virtual void OnEvent(Events::Event& Event) override;

		virtual void OnRender(void) const override;
		virtual void OnRenderGui(void) override;

	};
}