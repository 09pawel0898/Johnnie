#pragma once

#include "../../IWindow.hpp"

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
		virtual FORCEINLINE void* GetNativeWindow(void)	const override 
		{ 
			return m_WindowHandle; 
		}
		
		/** Events */
		virtual void InitEvents(void) override;
		virtual void SetEventCallback(EventCallback const& Callback) override;

		/** Update */
		virtual void OnTick(void) override;

	public:
		virtual void SetVSync(bool Enabled) override;

		virtual FORCEINLINE uint16_t GetHeight(void)	const override { return m_Properties.Height;}
		virtual FORCEINLINE uint16_t GetWidth(void)		const override { return m_Properties.Width;	}
	};

	FORCEINLINE void WindowsWindow::SetEventCallback(EventCallback const& Callback)
	{
		m_Properties.EventCallback = Callback;
	}
}