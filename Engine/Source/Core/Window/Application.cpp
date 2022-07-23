#include "Application.hpp"
#include "../Debug/Log.hpp"
#include "../../Events/WindowEvent.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"
//#include "../Graphics/Renderer.h"
//#include "../Graphics/Texture.h"

namespace Engine::Core
{
    #define BIND_APP_EVENT_FUNCTION(x) std::bind(&Application::x, this,std::placeholders::_1)

    std::shared_ptr<Application> Application::s_Instance = nullptr;

    void Application::OnEvent(Events::Event& Event)
    {
        Events::EventDispatcher dispatcher(Event);
        dispatcher.Dispatch<Events::WindowClosedEvent>(BIND_APP_EVENT_FUNCTION(OnWindowClosed));

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
        DEFINE_CONSOLE_LOG_CATEGORY(States);
        DEFINE_CONSOLE_LOG_CATEGORY(Events);
        
        m_Window = Window::Create(WindowProperties);
        m_Window->SetEventCallback(BIND_APP_EVENT_FUNCTION(OnEvent));
        
        //m_TextureManager = std::make_shared<TextureManager>();
        m_StateManager = StateManager::Create(State::Context(m_Window));
    }

    void Application::Run(void)
    { 
        LOG(Core, Trace, "Application::Run()");
        //Renderer::Init(m_Window->GetWidth(), m_Window->GetHeight());
        //Renderer::EnableBlending();

#if (_MSC_VER >= 1910)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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

        ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (m_bRunning)
        {
            tFrameStart = TimePoint::now();

            tElapsedTime = std::chrono::duration<double, std::milli>
                (tFrameStart - tLastUpdate).count();

            if (tElapsedTime >= tMinTimePerFrame)
            {
                m_Window->OnTick();

                m_StateManager->OnTick(m_DeltaTime);
                m_StateManager->OnRender();

                m_Window->OnBeginImGuiFrame();
                {
                    if (bool demoWindow = true; demoWindow)
                    {
                        ImGui::ShowDemoWindow(&demoWindow);
                    }
                }
                m_Window->OnEndImGuiFrame();
               
                glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
                glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
                glClear(GL_COLOR_BUFFER_BIT);
                //Renderer::Clear();
                m_Window->OnRenderImGuiFrame();
                
                tLastUpdate = TimePoint::now();
                updateStats();
            }
        }
    }
}