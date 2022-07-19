#pragma once

#include "../CoreMinimal.hpp"
#include "../../Events/Event.hpp"

struct GLFWwindow;
struct ImGuiIO;

namespace Engine::Core
{	
	using EventCallback = std::function<void(Events::Event&)>;

	struct WindowProperties
	{
		std::string		Title;
		
		unsigned		Width;
		unsigned		Height;		
		bool			bEnableVSync;

		EventCallback	EventCallback;

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
		ImGuiIO*			m_ImGuiIO;

		void InitEvents(void);
		bool InitOpenGLContext(void);
		bool InitImGuiContext(void);
		bool InitWindowHandle(WindowProperties const& Properties);
		void InitProperties(WindowProperties const& Properties);

	public:
		[[nodiscard]] 
		static std::shared_ptr<Window> Create(WindowProperties const& Properties = WindowProperties());
		
		void OnBeginImGuiFrame();
		void OnEndImGuiFrame();
		void OnRenderImGuiFrame();
		void OnTick(void);

	public:
		void SetEventCallback(EventCallback const& Callback);
		void SetVSync(bool Enabled);

		inline GLFWwindow* GetWindowHandle(void)	const { return m_WindowHandle;		}
		inline uint16_t GetHeight(void)				const { return m_Properties.Height; }
		inline uint16_t GetWidth(void)				const { return m_Properties.Width;	}
	};

	inline void Window::SetEventCallback(EventCallback const& Callback)
	{
		m_Properties.EventCallback = Callback;
	}
}