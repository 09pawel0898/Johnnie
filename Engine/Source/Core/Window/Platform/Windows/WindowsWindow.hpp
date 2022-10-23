#pragma once

#include "../../IWindow.hpp"
#include "../CoreGLFW.hpp"

struct GLFWwindow;
struct ImGuiIO;

namespace Engine::Core
{	
	class WindowsWindow final : public IWindow
	{
	public:
		explicit WindowsWindow(WindowProperties const& Properties);
		~WindowsWindow();

	private:
		/** Gui */
		ImGuiIO* m_ImGuiIO;

		/** Window Properties */
		WindowProperties m_Properties;
		void InitProperties(WindowProperties const& Properties);
		
		/** GLFW Window */
		GLFWwindow* m_WindowHandle;

		virtual bool InitNativeWindow(WindowProperties const& Properties) override;
		virtual void* GetNativeWindow(void)	const override 
		{ 
			return m_WindowHandle; 
		}
		
		/** Events */
		virtual void InitEvents(void) override;
		virtual void SetEventCallback(EventCallback const& Callback) override;

		/** Update */
		virtual void SwapBuffers(void) override;

	public:
		virtual void SetVSync(bool Enabled) override;

		virtual uint16_t GetHeight(void)	const override { return m_Properties.Height;}
		virtual uint16_t GetWidth(void)		const override { return m_Properties.Width;	}

		virtual bool IsResizeable(void) const override;
	};

	FORCEINLINE void WindowsWindow::SetEventCallback(EventCallback const& Callback)
	{
		m_Properties.EventCallback = Callback;
	}

	FORCEINLINE bool WindowsWindow::IsResizeable(void) const
	{
		return (bool)GLFW::WINDOW_RESIZEABLE;
	}
}