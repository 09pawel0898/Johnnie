#include "JohnnieLayer.hpp"

#include <Engine/Gui.hpp>
#include <Engine/System.hpp>
#include <Engine/Utilities.hpp>
#include <Engine/Renderer.hpp>
#include <Engine/CoreObject.hpp>
#include <Engine/Camera.hpp>

#include "Gui/SystemStatisticsWidget.hpp"
#include "Gui/CameraDataWidget.hpp"

/// test
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/RHI/Resources/RHIBuffers.hpp"
#include "Renderer/RHI/Resources/RHIVertexArray.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/RHI/RHICommand.hpp"
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
/// 

JohnnieLayer::JohnnieLayer(std::string_view Name) noexcept
	:	Layer(Name)
{
    m_Camera = NewObject<OFloatingCamera>(45.0f, (float)(1280 / 720), 0.1f, 100.0f);
}

void JohnnieLayer::OnAwake(void)
{
	InitGui();

    CameraController::Get()->SetViewTarget(m_Camera);
}

void JohnnieLayer::OnDetach(void)
{}

static void Test()
{
    static unsigned int VAO = 0;
    static uint32_t shaderProgram = 0;
    static bool init = false;
    static std::shared_ptr<RHIShader> s = nullptr;
    static std::shared_ptr<RHIVertexArray> _vao;
    static std::shared_ptr<RHITexture2D> texture;
    static std::shared_ptr<RHITexture2D> texture2;


    static float vertices[] =
    { //     COORDINATES         /      COLORS      /      TexCoord  //
         -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
         -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
          0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
          0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
          0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	0.5f, 1.0f
    };

    static uint32_t indices[] =
    {
         0, 1, 2,
         0, 2, 3,
         0, 1, 4,
         1, 2, 4,
         2, 3, 4,
         3, 0, 4
    };

    if (!init)
    {
        _vao = RHI::RHIVertexArray::Create();
        //_vao->Bind();

        RHIVertexBufferElement aPosition = RHIVertexBufferElement(RHIElementType::Float3, "aPosition");
        RHIVertexBufferElement aColor = RHIVertexBufferElement(RHIElementType::Float3, "aColor");
        RHIVertexBufferElement aTexCoord = RHIVertexBufferElement(RHIElementType::Float2, "aTextCoord");

        RHIVertexBufferLayout _layout{ aPosition, aColor, aTexCoord };

        auto _vbo = RHI::RHIVertexBuffer::Create(vertices, sizeof(vertices));

        //_vbo->Bind();

        _vbo->SetLayout(std::make_unique<RHIVertexBufferLayout>(std::move(_layout)));

        auto _ib = RHIIndexBuffer::Create(indices, sizeof(indices) / sizeof(int));
        _ib->Unbind();

        _vao->SetIndexBuffer(std::move(_ib));
        _vao->AddVertexBuffer(std::move(_vbo));



        texture = RHI::RHITexture2D::Create("Assets/Textures/texture.jpg");
        texture->Bind(0);
        texture2 = RHI::RHITexture2D::Create("Assets/Textures/texture2.png");
        texture2->Bind(1);

        s = RHI::RHIShader::Create("Basic", "Assets/Shaders/shader.glsl");
        s->Bind();

        s->SetInt("tex1", 0);
        s->SetInt("tex2", 1);

        //glGenVertexArrays(1, &VAO);
        //
        //GLuint VBO;
        //glGenBuffers(1, &VBO);
        //
        //glBindVertexArray(VAO);
        //
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //
        //
        //// Vertex attrib
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void const*)0);
        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void const*)(3*sizeof(float)));
        //glEnableVertexAttribArray(1);
        //
        //// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //
        //// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        //// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        //glBindVertexArray(0);
        s->Bind();
        init = true;
    }

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    static double prevTime = (double)(Utility::Time::now().time_since_epoch().count());
    static double rotation = 0.0;

    double nowTime = (double)(Utility::Time::now().time_since_epoch().count());

    if (nowTime - prevTime >= 1 / 60)
    {
        rotation += 0.5f;
        prevTime = nowTime;
    }

    model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, -0.5f, -3.0f));
    proj = glm::perspective(glm::radians(45.0f), (float)(1280 / 720), 0.1f, 100.0f);

    s->Bind();
    //s->SetMat4("model", model);
    //s->SetMat4("view", view);
    //s->SetMat4("proj", proj);

    _vao->Bind();

    texture->Bind(0);
    texture2->Bind(1);

    //RHICommand::DrawIndexed(_vao);

    Renderer::Get()->Draw(s, _vao, model);
}

void JohnnieLayer::OnRender(void) const
{
    Test();
}

void JohnnieLayer::OnTick(double DeltaTime)
{}

void JohnnieLayer::OnEvent(Events::Event& Event)
{}

void JohnnieLayer::InitGui(void)
{
    /** Register tickable widgets manually */
    m_SystemStatisticsWidget = std::make_shared<SystemStatisticsWidget>();
    TickableManager::Get()->RegisterTickable(m_SystemStatisticsWidget);

    m_CameraDataWidget = std::make_shared<CameraDataWidget>();
    TickableManager::Get()->RegisterTickable(m_CameraDataWidget);

    /** Init widgets delegates */
	m_MainMenuBarWidget.BindActionDelegate(MainMenuBarAction::Open,
	[]()
	{
		std::cout << "Open";
	});
}

void JohnnieLayer::OnRenderGui(void)
{
	m_MainMenuBarWidget.OnRenderGui();
	m_ConsoleLogWidget.OnRenderGui();
	m_SystemStatisticsWidget->OnRenderGui();
    m_CameraDataWidget->OnRenderGui();

	//static bool bShowDemoWindow = false;
	//ImGui::ShowDemoWindow(&bShowDemoWindow);
}
