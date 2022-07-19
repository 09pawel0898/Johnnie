#include <Engine/CoreMinimal.hpp>
#include <Engine/EntryPoint.hpp>

class InitialState : public States::State
{
public:
	InitialState(States::StateManager& StateManager, Context Context) noexcept
		:	State(StateManager,Context)
	{}

private:
	virtual void OnRender(void) const {};
	virtual void OnTick(double DeltaTime) {};
	virtual void OnEvent(Events::Event& Event) {};
};
#include <any>
class JohnnieApplication : public Core::Application
{
public:
	JohnnieApplication() noexcept
		:	Core::Application(Core::WindowProperties("Johnnie", 450, 600))
	{
		SetFPSLimit(120);
		DEFINE_CONSOLE_LOG_CATEGORY(Sandbox);

		GetStateManager()->RegisterState<InitialState>("InitialState");
		GetStateManager()->PushState("InitialState");
	}

	~JohnnieApplication() = default;
};

std::shared_ptr<Core::Application> Core::CreateApplication(void)
{
	return std::make_shared<JohnnieApplication>();
}