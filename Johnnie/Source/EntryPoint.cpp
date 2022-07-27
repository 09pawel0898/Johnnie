#include <Engine/CoreMinimal.hpp>
#include <Engine/EntryPoint.hpp>
#include <Engine/Debug.hpp>
#include <Engine/Gui.hpp>

class SandboxLayer : public Engine::Layer
{
public:
	explicit SandboxLayer(std::string_view Name) noexcept
		:	Layer(Name)
	{

	}

private:
	virtual void OnAwake(void) override 
	{}

	virtual void OnRender(void) const override{};
	virtual void OnTick(double DeltaTime) override {};
	virtual void OnEvent(Events::Event& Event) override 
	{
		std::cout << (int)Event.GetEventType();
	};
	virtual void OnRenderGui(void) override
	{
		Gui::Begin("sas");
		Gui::Text("Hello");
		Gui::End();

		if (bool demoWindow = true; demoWindow)
		{
			ImGui::ShowDemoWindow(&demoWindow);
		}
	}
};

class JohnnieApplication final : public Core::Application
{
public:
	JohnnieApplication()
		:	Core::Application(Core::WindowProperties("Johnnie", 1280, 720))
	{
		SetFPSLimit(120);
		GetLayerManager().PushLayer(std::make_unique<SandboxLayer>("Sandbox"));
	}

	~JohnnieApplication() = default;
};

std::shared_ptr<Core::Application> Core::CreateApplication(void)
{
	return std::make_shared<JohnnieApplication>();
}