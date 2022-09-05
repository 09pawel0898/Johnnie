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

		void InitTheme(void);
	public:
		explicit ImGuiLayer(std::string_view LayerName) noexcept
			:	Layer(LayerName)
		{}
		~ImGuiLayer() = default;

		virtual void OnAwake(void) override;
		virtual void OnDetach(void) override;
		virtual void OnEvent(Events::Event& Event) override;

		void OnNativeRenderGui(void);

		void BeginFrame(void);
		void EndFrame(void);

	public:
		ImGuiProperties& GetImGuiProperties(void)			{ return m_ImGuiProperties;	}
		ImGuiProperties const& GetImGuiProperties() const	{ return m_ImGuiProperties; }
	};
}
