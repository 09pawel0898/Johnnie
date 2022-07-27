#include "IWindow.hpp"

#ifdef PLATFORM_WINDOWS
#	include "Platform/Windows/WindowsWindow.hpp"
#endif

namespace Engine::Core
{
	WindowProperties::WindowProperties(std::string Title, unsigned Width, unsigned Height)
		:	Title(std::move(Title)),
			Width(Width),
			Height(Height),
			bEnableVSync(false),
			EventCallback(nullptr)
	{}

	std::unique_ptr<IWindow> IWindow::Create(WindowProperties const& Properties)
	{
#ifdef PLATFORM_WINDOWS

		return std::make_unique<WindowsWindow>(Properties);
#else
		CheckMsg(false, "Can't create window, unknown platform!");
		return nullptr;
#endif
	}
}