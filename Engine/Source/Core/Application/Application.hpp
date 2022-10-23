#pragma once

#include "Core/CoreMinimal.hpp"

#include "Core/Window/IWindow.hpp"
#include "Core/Layers/LayerManager.hpp"
#include "Core/Layers/EngineLayers/EngineBaseLayer.hpp"
#include "Renderer/ImGuiRenderer.hpp"

int main(void);

namespace Engine::Events
{
	class Event;
	class WindowClosedEvent;
}

namespace Engine::Core
{
	class Application
	{
		DEFINE_SUPER(Application);

	private:
		friend int ::main(void);
		static TSharedPtr<Application> s_Instance;

	public:
		static TSharedPtr<Application>& Get(void)
		{ 
			return s_Instance; 
		};

		explicit Application(const WindowProperties& WindowProperties = WindowProperties());

		virtual ~Application() = default;

		Application(Application const&) = delete;
		Application(Application &&) = delete;

		Application& operator =(Application const& ) = delete;
		Application& operator =(Application &&) = delete;

		/** Window */
	private:		
		TSharedPtr<IWindow> m_Window;

	public:
		TSharedPtr<IWindow> const& GetWindow(void) const 
		{ 
			return m_Window; 
		}

		ImGuiProperties& GetGuiProperties()
		{
			return ImGuiRenderer::Get()->GetImGuiProperties();
		};

		/** Layers */
	private:
		EngineBaseLayer m_EngineLayer{ "EngineLayer" };
		
		/** Initialization/Runtime */
	private:
		void Run(void);
		void UpdateFrame(void);

		void InitApplication(const WindowProperties& WindowProperties);
		void Shutdown(void);

	public:
		virtual void PostInitApplication(void);

	private:
		/** Statistics */
		bool		m_bRunning = true;
		double		m_DeltaTime = 0.0;
		double		m_FPS = 0.0;
		double 		m_FPSLIMIT = 9999.0;

	public:
		void SetFPSLimit(unsigned FpsLimit) { m_FPSLIMIT = FpsLimit; }
		double GetDT(void) const { return m_DeltaTime; }
		double GetFPS(void) const { return m_FPS; }

	public:
		/** Event Handling */
		void OnEvent(Events::Event& Event);
		bool OnWindowClosed(Events::WindowClosedEvent& Event);
	};

	[[nodiscard]] TSharedPtr<Application> CreateApplication();
}