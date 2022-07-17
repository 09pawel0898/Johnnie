#include <Engine/CoreMinimal.hpp>
#include <Engine/Core.hpp>

//#include "MainMenuState.h"
//#include "GameplayState.h"
//#include "GameLostState.h"

#include <Engine/EntryPoint.hpp>

class SandboxApplication : public Core::Application
{
public:
	SandboxApplication()
		:	Core::Application(Core::WindowProperties("Sandbox", 450, 600))
	{
		SetFPSLimit(120);
		DEFINE_CONSOLE_LOG_CATEGORY(Sandbox);

		//GetStateManager()->RegisterState<MainMenuState>("MainMenu");
		//GetStateManager()->RegisterState<GameplayState>("Gameplay");
		//GetStateManager()->RegisterState<GameLostState>("GameLost");
		//GetStateManager()->PushState("MainMenu");
	}

	~SandboxApplication() = default;
};

std::shared_ptr<Core::Application> Core::CreateApplication(void)
{
	return std::make_shared<SandboxApplication>();
}