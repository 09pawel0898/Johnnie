#pragma once

#include "Core/CoreMinimal.hpp"

#include "ImGui/ImGuiWidgetBase.hpp"
#include "Scene/SceneManager.hpp"

namespace Engine
{
	template <typename WidgetType, typename WidgetAction, typename... Args>
		requires std::is_base_of<ImGuiWidgetBase<WidgetAction>, WidgetType>::value
	FORCEINLINE std::shared_ptr<WidgetType> NewWidget(Args&&... _Args)
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();
		Check(activeScene);

		/** Create Widget */
		std::shared_ptr<WidgetType> newWidget = std::make_shared<WidgetType>(std::forward<Args>(_Args)...);

		activeScene->GetTickableManager().RegisterTickable(newWidget);
		activeScene->GetDrawableManager().RegisterWidget(static_pointer_cast<ImGuiWidgetBase<WidgetAction>>(newWidget));

		return newWidget;
	}
}