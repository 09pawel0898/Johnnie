#pragma once

#include "Core/CoreMinimal.hpp"
#include "Utilities/Singleton.hpp"
#include "Utilities/Delegates.hpp"

#include "Scene/Entities/Meshes/StaticMesh.hpp"

namespace Engine
{
	DECLARE_MULTICAST_DELEGATE(OnWindowResized, int32_t, int32_t);

	class ApplicationDelegates : public Singleton<ApplicationDelegates>
	{
	public:
		OnWindowResized OnWindowResized;
	};
}