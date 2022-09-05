#pragma once

namespace Engine
{
	class IDrawableWidget
	{
	public:
		virtual ~IDrawableWidget() {};

		virtual void OnRenderGui(void) = 0;
		virtual bool IsVisible(void) const = 0;
	};
}