#include "Application.hpp"
#include "Log/Log.hpp"
#include "Events/WindowEvent.hpp"
#include "System/System.hpp"
#include "Renderer/Renderer.hpp"

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
  
    void Application::Run(void)
    { 
        LOG(Core, Trace, "Application::Run");

        using TimePoint = std::chrono::high_resolution_clock;
        std::chrono::steady_clock::time_point tFrameStart, tLastUpdate = TimePoint::now();
        
        double tMinTimePerFrame = 1000.0 / m_FPSLIMIT;

        Renderer::GetRHI()->SetClearColor(glm::vec4(0.45f, 0.55f, 0.60f, 1.00f));
        Renderer::GetRHI()->SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());

        while (m_bRunning)
        {
            tFrameStart = TimePoint::now();

            m_DeltaTime = std::chrono::duration<double, std::milli>
                (tFrameStart - tLastUpdate).count();

            if (m_DeltaTime >= tMinTimePerFrame)
            {
                {
                    for (auto& layer : *m_LayerManager)
                    {
                        layer->OnTick(m_DeltaTime);
                    }
                
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

                Renderer::GetRHI()->Clear();

                tLastUpdate = TimePoint::now();
                m_FPS = (1.0 / m_DeltaTime) * 1000;;
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
        InitImGuiLayer();
    }

    void Application::Shutdown(void)
    {
        System::Shutdown();
        Renderer::Shutdown();
    }
}