#pragma once

#include "Core/CoreMinimal.hpp"
#include "Primitives/Object.hpp"
#include "Primitives/Tickable.hpp"

namespace Engine
{
	template <typename ObjectType, typename... Args>
		requires std::is_base_of<Object, ObjectType>::value
	FORCEINLINE std::shared_ptr<ObjectType> NewObject(Args&&... args)
	{
		/** Allocate object */
		std::shared_ptr<ObjectType> newObject = std::make_shared<ObjectType>(std::forward<Args>(args)...);

		/** Register if tickable */
		if (auto tickableObject = static_pointer_cast<Tickable>(newObject))
		{
			TickableManager::Get()->RegisterTickable(tickableObject);
		}
		return newObject;
	}
}