#pragma once

#include "../../Events/IEventListener.hpp"

namespace Engine
{
	class Layer : public Events::IEventListener
	{
	private:
		std::string_view m_LayerName;
	
	public:
		Layer() = delete;
		explicit Layer(std::string_view LayerName) noexcept
			:	m_LayerName(LayerName)
		{}
		virtual ~Layer() = default;

		Layer(const Layer& rhs) = delete;
		Layer(Layer&& rhs) = delete;
		
		Layer& operator = (const Layer& rhs) = delete;
		Layer& operator = (Layer&& rhs) = delete;

		std::string_view GetName(void) const
		{
			return m_LayerName;
		}

	public:
		virtual void OnAwake(void) {}
		virtual void OnDetach(void) {}

		virtual void OnTick(double DeltaTime) {}
		virtual void OnRender(void) const {}
		virtual void OnRenderGui(void) {}

		virtual void OnEvent(Events::Event& Event) override {}
	};
}