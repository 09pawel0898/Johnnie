#pragma once

#include "Events/Event.hpp"
#include "Core/Layers/Layer.hpp"

#include "ImGuiProperties.hpp"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	private:
		ImGuiProperties m_ImGuiProperties{};

	public:
		explicit ImGuiLayer(std::string_view LayerName) noexcept
			:	Layer(LayerName)
		{}
		~ImGuiLayer() = default;

		virtual void OnAwake() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Events::Event& Event) override;

		void OnNativeRenderGui();

		void BeginFrame();
		void EndFrame();

	public:
		ImGuiProperties& GetImGuiProperties()				{ return m_ImGuiProperties;	}
		ImGuiProperties const& GetImGuiProperties() const	{ return m_ImGuiProperties; }
	};
}
