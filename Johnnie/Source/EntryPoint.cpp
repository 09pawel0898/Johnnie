#include <Engine/CoreMinimal.hpp>
#include <Engine/EntryPoint.hpp>

#include "JohnnieLayer.hpp"

class JohnnieApplication final : public Core::Application
{
public:
	JohnnieApplication()
		:	Core::Application(Core::WindowProperties("Johnnie", 1280, 720))
	{
		SetFPSLimit(120);
		GetLayerManager().PushLayer(std::make_unique<JohnnieLayer>("JohnnieLayer"));
	}

	~JohnnieApplication() = default;
};

std::shared_ptr<Core::Application> Core::CreateApplication(void)
{
	return std::make_shared<JohnnieApplication>();
}