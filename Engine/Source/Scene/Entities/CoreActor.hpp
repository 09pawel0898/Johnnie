#pragma once

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
		auto& activeScene = SceneManager::Get()->GetActiveScene();
		CheckMsg(activeScene != nullptr,"No active scene.");

		/** Create object */
		TSharedPtr<ActorType> newActor = 
			MakeShared<ActorType>(Forward<Args>(args)...);

		activeScene->GetTickableManager().RegisterTickable(newActor);		
		activeScene->GetDrawableManager().RegisterActor(newActor);
		newActor->OnConstruct();

		return newActor;
	}
}