#include "EnginePCH.hpp"

#include "IWindow.hpp"

#ifdef PLATFORM_WINDOWS
#	include "Platform/Windows/WindowsWindow.hpp"
#endif

namespace Engine::Core
{
	WindowProperties::WindowProperties(std::string Title, unsigned Width, unsigned Height)
		:	Title(MoveTemp(Title)),
			Width(Width),
			Height(Height),
			bEnableVSync(false),
			EventCallback(nullptr)
	{}

	TUniquePtr<IWindow> IWindow::Create(WindowProperties const& Properties)
	{
#ifdef PLATFORM_WINDOWS

		return MakeUnique<WindowsWindow>(Properties);
#else
		CheckMsg(false, "Can't create window, unknown platform!");
		return nullptr;
#endif
	}
}