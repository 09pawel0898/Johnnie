#pragma once

#include "Core/CoreMinimal.hpp"

#include "Core/Window/IWindow.hpp"
#include "Core/Layers/LayerManager.hpp"
#include "Core/Layers/EngineLayers/EngineBaseLayer.hpp"
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
		DEFINE_SUPER(Application);

	private:
		friend int ::main(void);
		static std::shared_ptr<Application> s_Instance;

	public:
		static std::shared_ptr<Application>& Get(void)
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
		using WindowPointer	= std::shared_ptr<IWindow>;
		WindowPointer m_Window;

	public:
		WindowPointer const& GetWindow(void) const { return m_Window; }

		/** Layers */
	private:
		std::unique_ptr<LayerManager>	m_LayerManager;
		void InitLayerManager(void);
		
		std::shared_ptr<ImGuiLayer>		m_ImGuiLayer;
		void InitImGuiLayer(void);

		std::shared_ptr<EngineBaseLayer> m_EngineBaseLayer;
		void InitEngineLayer(void);

	public:
		ImGuiProperties& GetGuiProperties() { return m_ImGuiLayer->GetImGuiProperties(); };
		LayerManager& GetLayerManager(void) { return *m_LayerManager; }
		
		/** Initialization/Runtime */
	private:
		void Run(void);
		void InitApplication(const WindowProperties& WindowProperties);
		void Shutdown(void);
	public:
		virtual void PostInitApplication(void);

	private:
		/** Statistics */
		bool		m_bRunning = true;
		double		m_DeltaTime = 0.0;
		double		m_FPS = 0.0;
		unsigned	m_FPSLIMIT = 9999;

	public:
		void SetFPSLimit(unsigned FpsLimit) { m_FPSLIMIT = FpsLimit; }
		double GetDT(void) const { return m_DeltaTime; }
		double GetFPS(void) const { return m_FPS; }

	public:
		/** Event Handling */
		void OnEvent(Events::Event& Event);
		bool OnWindowClosed(Events::WindowClosedEvent& Event);
	};

	[[nodiscard]] std::shared_ptr<Application> CreateApplication();
}