#pragma once

#include "Core/CoreMinimal.hpp"

#include "Primitives/Actor.hpp"
#include "Lights/PointLight.hpp"
#include "Scene/SceneManager.hpp"

namespace Engine
{
	template <typename ActorType, typename... Args>
		requires std::is_base_of<Actor, ActorType>::value
	FORCEINLINE std::shared_ptr<ActorType> NewActor(Args&&... args)
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();
		CheckMsg(activeScene != nullptr,"No active scene.");

		/** Create object */
		std::shared_ptr<ActorType> newActor = 
			std::make_shared<ActorType>(std::forward<Args>(args)...);

		activeScene->GetTickableManager().RegisterTickable(newActor);		
		activeScene->GetDrawableManager().RegisterActor(newActor);
		newActor->OnConstruct();

		return newActor;
	}
}