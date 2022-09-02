#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"
#include <glm/glm.hpp>

enum class CameraDataAction : uint8_t
{};

class WCameraDataWidget final : public ImGuiWidgetBase<CameraDataAction>
{
public:
	WCameraDataWidget();

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

	bool m_bDefaultValuesInitialized = false;
	CameraMutableData m_CameraMutableData;
};