#include "EnginePCH.hpp"

#include "Application.hpp"
#include "Log/Log.hpp"
#include "Events/WindowEvent.hpp"
#include "System/System.hpp"
#include "Renderer/Renderer.hpp"
#include "Utilities/TimingUtility.hpp"
#include "Core/Debug/ProfileMacros.hpp"

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
        const double maxPeriod = 1.0 / m_FPSLIMIT;

        Utility::TimePoint tFrameStart, tLastUpdate = Utility::Time::now();

        Renderer::Get()->InitializeViewport(glm::i32vec4(0, 0, m_Window->GetWidth(), m_Window->GetHeight()));
       
        Renderer::Get()->InitializeFramebuffer("RenderWorld", 
            RHIFrameBufferSpecification(m_Window->GetWidth(), m_Window->GetHeight(), 8, 
                { 
                    {RHIFrameBufferAttachmentType::Color,RHIFrameBufferAttachmentTextureFormat::RGBA8},
                    {RHIFrameBufferAttachmentType::DepthStencil,RHIFrameBufferAttachmentTextureFormat::DEPTH24STENCIL8}
                },RHIFaceCullingType::Back));

        Renderer::Get()->InitializeFramebuffer("RenderWorldSingleSample",
            RHIFrameBufferSpecification(m_Window->GetWidth(), m_Window->GetHeight(), 1,
                {
                    {RHIFrameBufferAttachmentType::Color,RHIFrameBufferAttachmentTextureFormat::RGBA8}
                }));
        
        Renderer::Get()->InitializeFramebuffer("ShadowMap",
            RHIFrameBufferSpecification(2048, 2048, 1,
                {
                    {RHIFrameBufferAttachmentType::Depth,RHIFrameBufferAttachmentTextureFormat::DEPTH16}
                }, RHIFaceCullingType::None));

        Renderer::Get()->GetTexture2DManager().LoadResource("Assets/Textures/DepthTexture.png");

        while (m_bRunning)
        {
            tFrameStart = Utility::Time::now();

            m_DeltaTime = (double)(std::chrono::duration<double, std::milli>
                (tFrameStart - tLastUpdate).count() / 1000);

            if (m_DeltaTime >= maxPeriod)
            {
                m_FPS = (1.0 / m_DeltaTime);
                tLastUpdate = tFrameStart;

                UpdateFrame();
            }
        }
        Shutdown();
    }

    void Application::UpdateFrame(void)
    {
        PROFILE_SCOPE("RendererStats_FrameDuration");

        {
            PROFILE_SCOPE("RendererStats_TickDuration");
            for (auto& layer : *m_LayerManager)
            {
                layer->OnTick(m_DeltaTime);
            }
        }


        // Render ShadowMap //

        {
            Renderer::Get()->bIsRenderingShadowMap = true;
            PROFILE_SCOPE("RendererStats_RenderShadowMap");

            Renderer::Get()->BindFramebuffer("ShadowMap");
            for (auto& layer : *m_LayerManager)
            {
                layer->OnRender();
            }

            Renderer::Get()->bIsRenderingShadowMap = false;
        }

        // //

        Renderer::Get()->OnBeginRenderingFrame();
        {
            PROFILE_SCOPE("RendererStats_RenderDuration");
            {
                PROFILE_SCOPE("RendererStats_RenderWorldDuration");

                Renderer::Get()->BindFramebuffer("RenderWorld");
                for (auto& layer : *m_LayerManager)
                {
                    layer->OnRender();
                }
            }
            Renderer::Get()->GetFramebuffer("RenderWorld")->ResolveToFramebuffer(
                Renderer::Get()->GetFramebuffer("RenderWorldSingleSample"));

            {
                PROFILE_SCOPE("RendererStats_RenderGUIDuration");

                Renderer::Get()->BindDefaultFramebuffer();
                m_ImGuiLayer->BeginFrame();
                for (auto& layer : *m_LayerManager)
                {
                    layer->OnRenderGui();
                }
                m_ImGuiLayer->EndFrame();
            }

            {
                m_Window->OnTick();
                //Renderer::Get()->Clear();
            }
        }
        Renderer::Get()->OnEndRenderingFrame();
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