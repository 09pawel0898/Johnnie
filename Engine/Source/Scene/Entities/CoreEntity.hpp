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
		auto& ActiveScene = SceneManager::Get()->GetActiveScene();
		Check(ActiveScene);

		TSharedPtr<WidgetType> NewWidget = MakeShared<WidgetType>(Forward<Args>(_Args)...);

		ActiveScene->GetTickableManager().RegisterTickable(NewWidget);
		ActiveScene->GetDrawableManager().RegisterWidget(static_pointer_cast<ImGuiWidgetBase>(NewWidget));

		return NewWidget;
	}
}

#include "Core/CoreMinimal.hpp"

#include "Primitives/Actor.hpp"
#include "Lights/PointLight.hpp"
#include "Scene/SceneManager.hpp"

namespace Engine
{
	template <typename ActorType, typename... Args>
		requires std::is_base_of<Actor, ActorType>::value
	FORCEINLINE TSharedPtr<ActorType> NewActor(Args&&... args)
	{
		auto& ActiveScene = SceneManager::Get()->GetActiveScene();
		CheckMsg(ActiveScene != nullptr,"No active scene.");

		TSharedPtr<ActorType> NewActor = 
			MakeShared<ActorType>(Forward<Args>(args)...);

		ActiveScene->GetTickableManager().RegisterTickable(NewActor);		
		ActiveScene->GetDrawableManager().RegisterActor(NewActor);
		NewActor->OnConstruct();

		return NewActor;
	}
}

namespace Engine
{
	template <typename ObjectType, typename... Args>
	requires std::is_base_of<Object, ObjectType>::value
		FORCEINLINE TSharedPtr<ObjectType> NewObject(Args&&... args)
	{
		auto& ActiveScene = SceneManager::Get()->GetActiveScene();
		CheckMsg(ActiveScene != nullptr, "No active scene.");

		TSharedPtr<ObjectType> NewObject = MakeShared<ObjectType>(Forward<Args>(args)...);

		ActiveScene->GetTickableManager().RegisterTickable(NewObject);
		
		return NewObject;
	}
}