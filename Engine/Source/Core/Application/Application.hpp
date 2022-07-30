#pragma once
#include "Core/CoreMinimal.hpp"
#include "Core/Window/IWindow.hpp"
#include "Core/Layers/LayerManager.hpp"
#include "ImGui/ImGuiLayer.hpp"

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
		static FORCEINLINE std::shared_ptr<Application>& Get(void)
		{ 
			return s_Instance; 
		};

	private:		
		using WindowPointer	= std::shared_ptr<IWindow>;
		WindowPointer m_Window;

	public:
		FORCEINLINE WindowPointer const& GetWindow(void) const { return m_Window; }

	private:
		/** Layers */
		std::unique_ptr<LayerManager> m_LayerManager;
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;

		void InitLayerManager(void);
		void InitImGuiLayer(void);

	public:
		FORCEINLINE LayerManager& GetLayerManager(void) { return *m_LayerManager; }

	public:
		explicit Application(const WindowProperties& WindowProperties = WindowProperties());
		virtual ~Application() = default;

		Application(const Application& App) = delete;
		Application(Application&& App)		= delete;

		Application& operator =(const Application& App) = delete;
		Application& operator =(Application&& App)		= delete;
	
		void Run(void);

	private:
		void Shutdown(void);

		/** Statistics */
		bool		m_bRunning = true;
		double		m_DeltaTime = 0.0;
		double		m_FPS = 0.0;
		unsigned	m_FPSLIMIT = 9999;

	public:
		FORCEINLINE void SetFPSLimit(unsigned FpsLimit) { m_FPSLIMIT = FpsLimit; }
		FORCEINLINE double GetDT(void) const { return m_DeltaTime; }
		FORCEINLINE double GetFPS(void) const { return m_FPS; }

	public:
		/** Event Handling */
		void OnEvent(Events::Event& Event);
		bool OnWindowClosed(Events::WindowClosedEvent& Event);
	};

	[[nodiscard]] std::shared_ptr<Application> CreateApplication();
}