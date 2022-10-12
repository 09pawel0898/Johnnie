#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"
#include <glm/glm.hpp>

class WJohnnieCameraWidget final : public ImGuiWidgetBase
{
public:
	WJohnnieCameraWidget();

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;

private:
	struct CameraReadOnlyData
	{
		glm::vec3 Location;
		glm::vec3 Rotation;
		float CurrentAccelerationMod;
	};
	
	struct CameraMutableData
	{
		float Speed;
	};

	CameraReadOnlyData m_CameraData;
	CameraMutableData m_CameraMutableData;

	bool m_bDefaultValuesInitialized = false;
};