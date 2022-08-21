#include "EnginePCH.hpp"

#include "Application.hpp"
#include "Log/Log.hpp"
#include "Events/WindowEvent.hpp"
#include "System/System.hpp"
#include "Renderer/Renderer.hpp"
#include "Scene/Entities/Tickable.hpp"
#include "Utilities/TimingUtility.hpp"

namespace Engine::Core
{
    std::shared_ptr<Application> Application::s_Instance = nullptr;

    Application::Application(const WindowProperties& WindowProperties)
    {
        InitApplication(WindowProperties);
    }

    void Application::OnEvent(Events::Event& Event)
    {
        Events::EventDispatcher dispatcher(Event);
        dispatcher.Dispatch<Events::WindowClosedEvent>(BIND_FUNCTION(OnWindowClosed));

        if (Event.Handled())
        {
            return;
        }

        for (auto iter = m_LayerManager->end(); iter != m_LayerManager->begin();)
        {
            (*--iter)->OnEvent(Event);
            if (Event.Handled())
                return;
        }
    }

    bool Application::OnWindowClosed(Events::WindowClosedEvent& Event)
    {
        m_bRunning = false;
        return true;
    }

    void Application::InitLayerManager()
    {
        m_LayerManager = std::make_unique<LayerManager>();
    }

    void Application::InitImGuiLayer(void)
    {
        m_ImGuiLayer = std::make_shared<ImGuiLayer>("ImGuiLayer");
        m_LayerManager->PushOverlay(m_ImGuiLayer);
    }

    void Application::InitEngineLayer(void)
    {
        m_EngineBaseLayer = std::make_shared<EngineBaseLayer>("EngineBaseLayer");
        m_LayerManager->PushLayer(m_EngineBaseLayer);
    }
    
    void Application::Run(void)
    { 
        Utility::TimePoint tFrameStart, tLastUpdate = Utility::Time::now();
        double tMinTimePerFrame = (1000.0 / m_FPSLIMIT);

        Renderer::Get()->InitializeViewport(glm::i32vec4(0, 0, m_Window->GetWidth(), m_Window->GetHeight()));

        while (m_bRunning)
        {
            tFrameStart = Utility::Time::now();

            float timeSinceLastUpdate = (float)(std::chrono::duration<double, std::milli>
                (tFrameStart - tLastUpdate).count());

            m_DeltaTime = timeSinceLastUpdate / 1000.0f;

            if (timeSinceLastUpdate >= tMinTimePerFrame)
            {
                {
                    for (auto& layer : *m_LayerManager)
                    {
                        layer->OnTick(m_DeltaTime);
                    }
                    
                    TickableManager::Get()->UdateTickableEntities(m_DeltaTime);

                    for (auto& layer : *m_LayerManager)
                    {
                        layer->OnRender();
                    }
                }
                
                {
                    m_ImGuiLayer->BeginFrame();
                    for (auto& layer : *m_LayerManager)
                    {
                        layer->OnRenderGui();
                    }
                    m_ImGuiLayer->EndFrame();
                }
                m_Window->OnTick();

                Renderer::Get()->Clear();

                tLastUpdate = Utility::Time::now();
                m_FPS = (1.0 / m_DeltaTime);
            }

            
        }
        Shutdown();
    }

    void Application::InitApplication(const WindowProperties& WindowProperties)
    {
        Check(s_Instance == nullptr);

        Log::RegisterEngineLoggers();

        m_Window = IWindow::Create(WindowProperties);
        m_Window->SetEventCallback(BIND_FUNCTION(OnEvent));

        Renderer::Init(RHI::RenderingAPI::OpenGL);
        System::Init();

        InitLayerManager();

#if (_MSC_VER >= 1910)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    }

    void Application::PostInitApplication(void)
    {
        InitEngineLayer();
        InitImGuiLayer();
    }

    void Application::Shutdown(void)
    {
        System::Shutdown();
        Renderer::Shutdown();
    }
}