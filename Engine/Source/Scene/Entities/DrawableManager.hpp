#pragma once

#include "Core/CoreMinimal.hpp"

#include "Primitives/Actor.hpp"
#include "ImGui/ImGuiWidgetBase.hpp"
#include "Primitives/IDrawable.hpp"
#include "Primitives/IDrawableWidget.hpp"


namespace Engine
{
	enum class DrawableType : uint8_t
	{
		Actor,
		Widget
	};

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

		void RegisterWidget(std::shared_ptr<ImGuiWidgetBase> const& DrawableWidget);

		void DrawActors(void);
		void DrawWidgets(void);

	private:
		void UnRegisterDrawables(DrawableType Type, std::vector<OUUID> const& PendingIDsToUnregister);
	};
}