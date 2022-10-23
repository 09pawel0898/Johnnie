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
    TSharedPtr<Application> Application::s_Instance = nullptr;

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

        m_EngineLayer.OnEvent(Event);
    }

    bool Application::OnWindowClosed(Events::WindowClosedEvent& Event)
    {
        m_bRunning = false;
        return true;
    }
    
    void Application::Run(void)
    { 
        const double maxPeriod = 1.0 / m_FPSLIMIT;

        Utility::TimePoint tFrameStart, tLastUpdate = Utility::Time::now();

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
        TUniquePtr<Renderer>& renderer = Renderer::Get();

        TUniquePtr<RHIFrameBuffer>& shadowMapFrameBuffer        = renderer->GetFramebuffer("ShadowMap");
        TUniquePtr<RHIFrameBuffer>& worldFrameBuffer            = renderer->GetFramebuffer("RenderWorld");
        TUniquePtr<RHIFrameBuffer>& worldResolvedFrameBuffer    = renderer->GetFramebuffer("RenderWorld_Resolved");

        Renderer::Get()->GetFramebuffer("ShadowMap");

        PROFILE_SCOPE("RendererStats_FrameDuration");

        {
            PROFILE_SCOPE("RendererStats_TickDuration");
            m_EngineLayer.OnTick(m_DeltaTime);
        }

        {
            PROFILE_SCOPE("RendererStats_RenderShadowMap");

            renderer->bIsRenderingShadowMap = true;

            shadowMapFrameBuffer->Bind();
            m_EngineLayer.OnRender();

            renderer->bIsRenderingShadowMap = false;
        }

        Renderer::Get()->OnBeginRenderingFrame();
        {
            PROFILE_SCOPE("RendererStats_RenderDuration");
            {
                PROFILE_SCOPE("RendererStats_RenderWorldDuration");

                worldFrameBuffer->Bind();
                m_EngineLayer.OnRender();

                worldFrameBuffer->ResolveToFramebuffer(worldResolvedFrameBuffer);
            }
           
            {
                PROFILE_SCOPE("RendererStats_RenderGUIDuration");

                renderer->BindDefaultFramebuffer();

                ImGuiRenderer::Get()->BeginFrame();
                m_EngineLayer.OnRenderGui();
                ImGuiRenderer::Get()->EndFrame();
            }

            m_Window->SwapBuffers();
        }
        Renderer::Get()->OnEndRenderingFrame();
    }

    void Application::InitApplication(const WindowProperties& WindowProperties)
    {
        Check(s_Instance == nullptr);

        LogManager::RegisterEngineLoggers();

        m_Window = IWindow::Create(WindowProperties);
        m_Window->SetEventCallback(BIND_FUNCTION(OnEvent));

#if (_MSC_VER >= 1910)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    }

    void Application::PostInitApplication(void)
    {
        Renderer::Init(RHI::RenderingAPI::OpenGL);
        
        Renderer::Get()->InitializeViewport(glm::i32vec4(0, 0, m_Window->GetWidth(), m_Window->GetHeight()));

        Renderer::Get()->InitializeFramebuffer("RenderWorld",
            RHIFrameBufferSpecification(m_Window->GetWidth(), m_Window->GetHeight(), 8,
                {
                    {RHIFrameBufferAttachmentType::Color,RHIFrameBufferAttachmentTextureFormat::RGBA8},
                    {RHIFrameBufferAttachmentType::DepthStencil,RHIFrameBufferAttachmentTextureFormat::DEPTH24STENCIL8}
                }, RHIFaceCullingType::Back));

        Renderer::Get()->InitializeFramebuffer("RenderWorld_Resolved",
            RHIFrameBufferSpecification(m_Window->GetWidth(), m_Window->GetHeight(), 1,
                {
                    {RHIFrameBufferAttachmentType::Color,RHIFrameBufferAttachmentTextureFormat::RGBA8}
                }));

        Renderer::Get()->InitializeFramebuffer("ShadowMap",
            RHIFrameBufferSpecification(2048, 2048, 1,
                {
                    {RHIFrameBufferAttachmentType::Depth,RHIFrameBufferAttachmentTextureFormat::DEPTH16}
                }, 
                RHIFaceCullingType::None));

        ImGuiRenderer::Init(RHI::RenderingAPI::OpenGL);
        System::Init();

        m_EngineLayer.OnAwake();
    }

    void Application::Shutdown(void)
    {
        m_EngineLayer.OnDetach();

        System::Shutdown();
        ImGuiRenderer::Shutdown();
        Renderer::Shutdown();
    }
}