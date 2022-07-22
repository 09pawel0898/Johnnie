#include <Engine/CoreMinimal.hpp>
#include <Engine/EntryPoint.hpp>
#include <Engine/Debug.hpp>

class InitialState : public States::State
{
public:
	InitialState(States::StateManager& StateManager, Context Context) noexcept
		:	State(StateManager,Context)
	{}

private:
	virtual void OnAwake(void) override 
	{
	
	}
	virtual void OnRender(void) const override{};
	virtual void OnTick(double DeltaTime) override {};
	virtual void OnEvent(Events::Event& Event) override {};
};

class JohnnieApplication final : public Core::Application
{
public:
	JohnnieApplication() noexcept
		:	Core::Application(Core::WindowProperties("Johnnie", 1280, 720))
	{
		SetFPSLimit(120);

		GetStateManager()->RegisterState<InitialState>("InitialState");
		GetStateManager()->PushState("InitialState");
	}

	~JohnnieApplication() = default;
};

std::shared_ptr<Core::Application> Core::CreateApplication(void)
{
	return std::make_shared<JohnnieApplication>();
}