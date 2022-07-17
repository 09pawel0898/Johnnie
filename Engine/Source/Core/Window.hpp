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

		WindowProperties(	std::string const& Title = "DefaultWindow", 
							unsigned Width = 1280, 
							unsigned Height = 720);
	};

	class Window
	{
	public:
		Window(WindowProperties const& Properties);
		~Window();

	private:
		WindowProperties	m_Properties;
		GLFWwindow*			m_WindowHandle;

		void InitProperties(WindowProperties const& Properties);
		void InitEvents(void);
		bool InitWindowHandle(WindowProperties const& Properties);
		bool InitOpenGL(void);

	public:
		static std::shared_ptr<Window> Create(WindowProperties const& Properties = WindowProperties());
		void OnTick(void);

	public:
		void SetEventCallback(ClosedEventCallback const& Callback);
		void SetVSync(bool Enabled);

		inline GLFWwindow* GetWindowHandle(void)	const { return m_WindowHandle;		}
		inline uint16_t GetHeight(void)				const { return m_Properties.Height; }
		inline uint16_t GetWidth(void)				const { return m_Properties.Width;	}
	};

	inline void Window::SetEventCallback(ClosedEventCallback const& Callback)
	{
		m_Properties.EventCallback = Callback;
	}
}