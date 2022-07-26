#pragma once

#include "../Events/Event.hpp"
#include "../Core/Layers/Layer.hpp"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	public:
		explicit ImGuiLayer(std::string_view LayerName) noexcept
			:	Layer(LayerName)
		{}

		virtual void OnAwake() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Events::Event& Event) override;

		void BeginFrame();
		void EndFrame();
	};
}
