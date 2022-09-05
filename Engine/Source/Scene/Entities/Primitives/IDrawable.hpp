#pragma once

namespace Engine
{
	class IDrawable
	{
	public:
		virtual ~IDrawable() {};

		virtual void Draw(void) const = 0;
		virtual bool IsVisible(void) const = 0;
	};
}