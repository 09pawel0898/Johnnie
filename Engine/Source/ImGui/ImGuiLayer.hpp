#pragma once

#include "../Events/Event.hpp"
#include "../Core/Layers/Layer.hpp"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	private:
		bool m_BlockEvents = true;

	public:
		explicit ImGuiLayer(std::string_view LayerName) noexcept
			:	Layer(LayerName)
		{}

		virtual void OnAwake() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Events::Event& Event) override;

		void BeginFrame();
		void EndFrame();

		inline void BlockEvents(bool Block) { m_BlockEvents = Block; }
	};
}
