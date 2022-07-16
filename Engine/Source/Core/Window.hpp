#pragma once

#include "CoreMinimal.hpp"
#include "../Events/Event.hpp"

struct GLFWwindow;

namespace Engine::Core
{	
	using ClosedEventCallback = std::function<void(Events::Event&)>;

	struct WindowProperties
	{
		std::string			Title;
		
		unsigned			Width;
		unsigned			Height;		
		bool				bEnableVSync;

		ClosedEventCallback	EventCallback;

		WindowProperties(const std::string& Title = "DefaultWindow", unsigned Width = 1280, unsigned Height = 720);
	};

	class Window
	{
	public:
		Window(const WindowProperties& Properties);
		~Window();

	private:
		WindowProperties	m_Properties;
		GLFWwindow*			m_WindowHandle;

		void InitProperties(const WindowProperties& Properties);
		void InitEvents();
		bool InitWindowHandle();
		bool InitOpenGL();

	public:
		static std::shared_ptr<Window> Create(const WindowProperties& Properties = WindowProperties());
		void OnTick(void);

	public:
		void SetEventCallback(const ClosedEventCallback& Callback);
		void SetVSync(bool enabled);

		inline GLFWwindow* GetWindowHandle(void)	const { return m_WindowHandle;		}
		inline uint16_t GetHeight()					const { return m_Properties.Height; }
		inline uint16_t GetWidth()					const { return m_Properties.Width;	}
	};

	inline void Window::SetEventCallback(const ClosedEventCallback& Callback)
	{
		m_Properties.EventCallback = Callback;
	}
}