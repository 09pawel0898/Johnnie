#include "Application.hpp"
#include "Log/Log.hpp"
#include "../Events/WindowEvent.hpp"

//#include "../Graphics/Renderer.h"
//#include "../Graphics/Texture.h"

namespace Engine::Core
{
    #define BIND_APP_EVENT_FUNCTION(x) std::bind(&Application::x, this,std::placeholders::_1)

    std::shared_ptr<Application> Application::s_Instance = nullptr;

    void Application::OnEvent(Events::Event& Event)
    {
        Events::EventDispatcher dispatcher(Event);
        dispatcher.Dipatch<Events::WindowClosedEvent>(BIND_APP_EVENT_FUNCTION(OnWindowClosed));

        if (Event.Handled())
        {
            return;
        }
        m_StateManager->OnEvent(Event);
    }

    bool Application::OnWindowClosed(Events::WindowClosedEvent& Event)
    {
        m_bRunning = false;
        return false;
    }

    Application::Application(const WindowProperties& WindowProperties)
    {
        using namespace States;
        
        DEFINE_CONSOLE_LOG_CATEGORY(Core);

        m_Window = Window::Create(WindowProperties);
        m_Window->SetEventCallback(BIND_APP_EVENT_FUNCTION(OnEvent));
        
        //m_TextureManager = std::make_shared<TextureManager>();
        m_StateManager = StateManager::Create(State::Context(m_Window));
    }

    void Application::Run()
    { 
        //Renderer::Init(m_Window->GetWidth(), m_Window->GetHeight());
        //Renderer::EnableBlending();

        using TimePoint = std::chrono::high_resolution_clock;

        std::chrono::steady_clock::time_point tFrameStart, tLastUpdate;

        tLastUpdate = TimePoint::now();
        double tElapsedTime{ 0.0 };
        double tMinTimePerFrame = 1000.0 / m_FPSLIMIT;

        auto updateStats = [&, this]
        {
            m_DeltaTime = tElapsedTime;
            m_FPS = (1.0 / m_DeltaTime) * 1000;
        };

        while (m_bRunning)
        {
            tFrameStart = TimePoint::now();

            tElapsedTime = std::chrono::duration<double, std::milli>
                (tFrameStart - tLastUpdate).count();

            if (tElapsedTime >= tMinTimePerFrame)
            {
                //Renderer::Clear();
                m_StateManager->OnTick(m_DeltaTime);
                m_StateManager->OnRender();
                m_Window->OnTick();
                
                tLastUpdate = TimePoint::now();
                updateStats();
            }
        }
    }
}