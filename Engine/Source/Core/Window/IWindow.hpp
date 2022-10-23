#pragma once

#include "Core/CoreMinimal.hpp"
#include "Events/Event.hpp"

namespace Engine::Core
{
	using EventCallback = std::function<void(Events::Event&)>;

	struct WindowProperties
	{
		std::string		Title;

		unsigned		Width;
		unsigned		Height;
		bool			bEnableVSync;

		EventCallback	EventCallback = nullptr;

		WindowProperties(	std::string Title = "DefaultWindow",
							unsigned Width = 1280,
							unsigned Height = 720);
	};

	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		[[nodiscard]]
		static TUniquePtr<IWindow> Create(WindowProperties const& Properties = WindowProperties());

	public:
		/** Native Window Handle */
		virtual bool InitNativeWindow(WindowProperties const& Properties) = 0;
		virtual void* GetNativeWindow(void) const = 0;

		/** Events */
		virtual void InitEvents(void) = 0;
		virtual void SetEventCallback(const EventCallback& Callback) = 0;

		/** Update */
		virtual void SwapBuffers(void) = 0;

		/** Properties */
		virtual uint16_t GetWidth(void)	 const = 0;
		virtual uint16_t GetHeight(void) const = 0;
		virtual void SetVSync(bool Enabled) = 0;
		virtual bool IsResizeable(void) const = 0;
	};
}