#pragma once

#include "ImGui/ImGuiWidgetBase.hpp"
#include <glm/glm.hpp>

enum class CameraDataAction : uint8_t
{};

class CameraDataWidget final : public ImGuiWidgetBase<CameraDataAction>
{
public:
	CameraDataWidget();

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
		glm::vec3 Speed;
		glm::vec3 Acceleration;
	};

	CameraReadOnlyData m_CameraData;
};