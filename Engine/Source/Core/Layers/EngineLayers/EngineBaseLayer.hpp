#pragma once

#include "../Layer.hpp"

namespace Engine
{
	namespace Core
	{
		class Application;
	}

	class EngineBaseLayer final : public Layer
	{
		friend class Core::Application;

	private:
		explicit EngineBaseLayer(std::string_view Name)
			:	Layer(Name)
		{}

		/** Layer Interface Impl*/
		virtual void OnAwake(void) override;
		virtual void OnDetach(void) override;

		virtual void OnTick(double DeltaTime) override;
		virtual void OnEvent(Events::Event& Event) override;

		virtual void OnRender(void) const override;
		virtual void OnRenderGui(void) override;

		void PrepareShaders(void);
	};
}