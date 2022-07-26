#pragma once
#include "../CoreMinimal.hpp"
#include "WindowsWindow.hpp"
#include "../Layers/LayerManager.hpp"
#include "../../ImGui/ImGuiLayer.hpp"

//#include "../Resources/ResourceManager.h"

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
	private:
		friend int ::main(void);
		static std::shared_ptr<Application> s_Instance;

	public:
		static inline std::shared_ptr<Application>& Get(void)
		{ 
			return s_Instance; 
		};

	private:		
		using WindowPointer	= std::shared_ptr<Window>;
		WindowPointer m_Window;

	public:
		inline WindowPointer const& GetWindow(void) const { return m_Window; }

	private:
		/** Layers */
		std::unique_ptr<LayerManager> m_LayerManager;
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;

		void InitLayerManager(void);
		void InitImGuiLayer(void);

	public:
		inline LayerManager& GetLayerManager(void) { return *m_LayerManager; }

	private:
		bool		m_bRunning = true;
		double		m_DeltaTime = 0.0;
		double		m_FPS = 0.0;
		unsigned	m_FPSLIMIT = 9999;

	public:
		explicit Application(const WindowProperties& WindowProperties = WindowProperties());
		virtual ~Application() = default;

		Application(const Application& App) = delete;
		Application(Application&& App)		= delete;

		Application& operator =(const Application& App) = delete;
		Application& operator =(Application&& App)		= delete;
	
	public:
		
		void OnEvent(Events::Event& Event);
		bool OnWindowClosed(Events::WindowClosedEvent& Event);

		void Run(void);

		inline void SetFPSLimit(unsigned FpsLimit)	{ m_FPSLIMIT = FpsLimit;}
		inline double GetDT(void) const				{ return m_DeltaTime;	}
		inline double GetFPS(void) const			{ return m_FPS;			}
	
	private:
		void Shutdown(void);
	};

	[[nodiscard]] std::shared_ptr<Application> CreateApplication();
}