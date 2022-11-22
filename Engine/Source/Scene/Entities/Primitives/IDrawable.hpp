#pragma once

namespace Engine
{
	class IDrawable
	{
	public:
		virtual ~IDrawable() {};

		virtual void Draw(void) = 0;
		virtual bool IsVisible(void) const = 0;
	};
}