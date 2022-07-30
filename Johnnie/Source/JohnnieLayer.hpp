#pragma once

#include <Engine/CoreMinimal.hpp>
#include <Engine/Layers.hpp>

class JohnnieLayer final : public Engine::Layer
{
public:
	explicit JohnnieLayer(std::string_view Name) noexcept;

private:
	virtual void OnAwake(void) override;
	virtual void OnDetach(void) override;

	virtual void OnTick(double DeltaTime) override;
	virtual void OnEvent(Events::Event& Event) override;
	
	virtual void OnRender(void) const override;
	virtual void OnRenderGui(void) override;
};