#pragma once

#include "CoreMinimal.hpp"

//#include <memory>

#include "Window.hpp"

#include "../States/StateManager.hpp"
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
		static std::shared_ptr<Application> s_Instance;

	public:
		static inline std::shared_ptr<Application>& Get(void)
		{ 
			return s_Instance; 
		};

	private:
		friend int ::main(void);
		
		using WindowPtr = std::shared_ptr<Window>;
		using StateManagerPtr = std::unique_ptr<States::StateManager>;
		//using TextureManagerPtr = std::shared_ptr<TextureManager>;
	
	private:
		WindowPtr			m_Window;
		StateManagerPtr		m_StateManager;
		//TextureManagerPtr m_TextureManager;
		
		bool		m_bRunning = true;
		double		m_DeltaTime = 0.0;
		double		m_FPS = 0.0;
		unsigned	m_FPSLIMIT = 9999;

	public:
		explicit Application(const WindowProperties& WindowProperties = WindowProperties());
		virtual ~Application() = default;

		Application(const Application& App) = delete;
		Application(Application&& App) = delete;

		Application& operator =(const Application& App) = delete;
		Application& operator =(Application&& App) = delete;
	
	public:
		
		void OnEvent(Events::Event& Event);
		bool OnWindowClosed(Events::WindowClosedEvent& Event);

		void Run();

		inline void SetFPSLimit(unsigned FpsLimit)		{ m_FPSLIMIT = FpsLimit; }

		inline double GetDT(void) const					{ return m_DeltaTime; }
		inline double GetFPS(void) const				{ return m_FPS; }
		inline WindowPtr const& GetWindow(void) const	{ return m_Window; }

		inline StateManagerPtr const& GetStateManager(void) const { return m_StateManager; }
		//inline const TextureManagerPtr& GetTextureManager(void) const { return m_TextureManager; }
	};

	std::shared_ptr<Application> CreateApplication();
}