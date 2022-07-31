#pragma once

namespace Engine
{
	struct ImGuiProperties
	{
	private:
		bool bDockingEnabled;

		friend class ImGuiLayer;
	public:
		void EnableDocking(void);
	};
}