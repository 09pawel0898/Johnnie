#pragma once

namespace Engine
{
	struct ImGuiProperties
	{
		friend class ImGuiRenderer;

	private:
		bool bDockingEnabled;

	public:
		void EnableDocking(void);
	};
}