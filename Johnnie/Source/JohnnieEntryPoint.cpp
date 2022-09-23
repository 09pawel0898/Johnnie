#include <Engine/CoreMinimal.hpp>
#include <Engine/EntryPoint.hpp>
#include <Engine/Scene.hpp>

#include "JohnnieScene.hpp"

class JohnnieApplication final : public Core::Application
{
public:
	JohnnieApplication()
		:	Core::Application(Core::WindowProperties("Johnnie", 1280, 720))
	{
		SetFPSLimit(60);
		SceneManager::Get()->SetScene(std::make_unique<JohnnieScene>());
	}

	virtual void PostInitApplication(void) override
	{
		Super::PostInitApplication();

		GetGuiProperties().EnableDocking();
	}

	~JohnnieApplication() = default;
};

std::shared_ptr<Core::Application> Core::CreateApplication(void)
{
	return std::make_shared<JohnnieApplication>();
}