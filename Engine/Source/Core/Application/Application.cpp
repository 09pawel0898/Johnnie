#include "EnginePCH.hpp"

#include "Application.hpp"
#include "Log/Log.hpp"
#include "Events/WindowEvent.hpp"
#include "System/System.hpp"
#include "Renderer/Renderer.hpp"

#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/RHI/Resources/RHIBuffers.hpp"

#include <glad/glad.h>

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
    
    static void Test()
    {
       static unsigned int VAO = 0;
       static GLuint shaderProgram = 0;
       static bool init = false;
       static std::unique_ptr<RHIShader> s = nullptr;
       
       if(!init)
       {
           float vertices[] = 
           {
                -0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,
                0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f,
                0.0f,  0.5f, 0.0f,  0.0f,0.0f,1.0f
           };
           
           //auto vbo = RHI::RHIVertexBuffer::Create(vertices, sizeof(vertices));

           s = RHI::RHIShader::Create("Basic", "Assets/Shaders/shader.glsl");
       
           glGenVertexArrays(1, &VAO);
       
           GLuint VBO;
           glGenBuffers(1, &VBO);

           glBindVertexArray(VAO);
       
           glBindBuffer(GL_ARRAY_BUFFER, VBO);
           glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
       
          
           // Vertex attrib
           glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
           glEnableVertexAttribArray(0);

           // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
           glBindBuffer(GL_ARRAY_BUFFER, 0);
       
           // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
           // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
           glBindVertexArray(0);
           init = true;
       }
       s->Bind();
       glBindVertexArray(VAO);
       glDrawArrays(GL_TRIANGLES, 0, 3);
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
                Test();
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