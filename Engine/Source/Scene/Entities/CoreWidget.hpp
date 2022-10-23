#pragma once

#include "Core/CoreMinimal.hpp"

#include "ImGui/ImGuiWidgetBase.hpp"
#include "Scene/SceneManager.hpp"

namespace Engine
{
	template <typename WidgetType, typename... Args>
		requires std::is_base_of<ImGuiWidgetBase, WidgetType>::value
	FORCEINLINE TSharedPtr<WidgetType> NewWidget(Args&&... _Args)
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();
		Check(activeScene);

		/** Create Widget */
		TSharedPtr<WidgetType> newWidget = MakeShared<WidgetType>(Forward<Args>(_Args)...);

		activeScene->GetTickableManager().RegisterTickable(newWidget);
		activeScene->GetDrawableManager().RegisterWidget(static_pointer_cast<ImGuiWidgetBase>(newWidget));

		return newWidget;
	}
}