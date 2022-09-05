#pragma once

#include "Core/CoreMinimal.hpp"

#include "Primitives/Actor.hpp"
#include "Scene/SceneManager.hpp"

namespace Engine
{
	template <typename ActorType, typename... Args>
		requires std::is_base_of<Actor, ActorType>::value
	FORCEINLINE std::shared_ptr<ActorType> NewActor(Args&&... args)
	{
		auto& activeScene = SceneManager::Get()->GetActiveScene();
		Check(activeScene);

		/** Create object */
		std::shared_ptr<ActorType> newActor = 
			std::make_shared<ActorType>(std::forward<Args>(args)...);

		activeScene->GetTickableManager().RegisterTickable(newActor);		
		activeScene->GetDrawableManager().RegisterActor(newActor);
		
		return newActor;
	}
}