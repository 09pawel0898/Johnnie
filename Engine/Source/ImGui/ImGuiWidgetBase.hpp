#pragma once

#include "Core/Debug/Asserts.hpp"
#include "../Scene/Entities/Primitives/Object.hpp"
#include "../Scene/Entities/Primitives/IDrawableWidget.hpp"

namespace Engine
{
	class ImGuiWidgetBase : public Object, public IDrawableWidget
	{
	private:
		bool m_bIsVisible = true;

		virtual void OnRenderGui(void) = 0;
		virtual void OnTick(double DeltaTime) {};

	public:
		virtual bool IsVisible(void) const override
		{
			return m_bIsVisible;
		}

		void SetVisibility(bool Visibility)
		{
			m_bIsVisible = Visibility;
		}
	};
}