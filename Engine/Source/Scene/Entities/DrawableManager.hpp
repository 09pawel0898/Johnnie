#pragma once

#include "Core/CoreMinimal.hpp"

#include "Primitives/Actor.hpp"
#include "ImGui/ImGuiWidgetBase.hpp"
#include "Primitives/IDrawable.hpp"
#include "Primitives/IDrawableWidget.hpp"


namespace Engine
{
	class DrawableManager final
	{
	private:
		std::vector<
			std::pair<OUUID, std::weak_ptr<IDrawable>>
		> m_DrawableObjects;
	
		std::vector<
			std::pair<OUUID, std::weak_ptr<IDrawableWidget>>
		> m_DrawableWidgets;
	
	public:
		DrawableManager() = default;

		DrawableManager(DrawableManager const&) = delete;
		DrawableManager& operator=(DrawableManager const&) = delete;

		DrawableManager(DrawableManager&&) = delete;
		DrawableManager& operator=(DrawableManager&&) = delete;

	public:
		void RegisterActor(std::shared_ptr<Actor> const& DrawableActor);

		template<typename ActionType>
		void RegisterWidget(std::shared_ptr<ImGuiWidgetBase<ActionType>> const& DrawableWidget);

		void DrawActors(void);
		void DrawWidgets(void);
	};

	template <typename ActionType>
	void DrawableManager::RegisterWidget(std::shared_ptr<ImGuiWidgetBase<ActionType>> const& DrawableWidget)
	{
		Check(m_DrawableWidgets.cend() ==
			std::find_if(m_DrawableWidgets.cbegin(), m_DrawableWidgets.cend(),
			[&DrawableWidget](std::pair<OUUID, std::weak_ptr<IDrawableWidget>> const& Element) -> bool
			{
				return DrawableWidget->GetUUID() == Element.first;
			}));

		m_DrawableWidgets.emplace_back(
			std::make_pair(DrawableWidget->GetUUID(), std::weak_ptr<IDrawableWidget>(DrawableWidget)));
	}
}