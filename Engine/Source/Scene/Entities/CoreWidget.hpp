#pragma once

#include "Core/CoreMinimal.hpp"

#include "ImGui/ImGuiWidgetBase.hpp"
#include "Primitives/Tickable.hpp"

namespace Engine
{
	template <typename WidgetType, typename WidgetAction, typename... Args>
		requires std::is_base_of<ImGuiWidgetBase<WidgetAction>, WidgetType>::value
	FORCEINLINE std::shared_ptr<WidgetType> NewWidget(Args&&... _Args)
	{
		/** Allocate object */
		std::shared_ptr<WidgetType> newWidget = std::make_shared<WidgetType>(std::forward<Args>(_Args)...);

		/** Register if tickable */
		if (auto tickableObject = static_pointer_cast<Tickable>(newWidget))
		{
			TickableManager::Get()->RegisterTickable(tickableObject);
		}
		return newWidget;
	}
}