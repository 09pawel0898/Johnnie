#pragma once

#include "Core/CoreMinimal.hpp"
#include "Utilities/Singleton.hpp"
#include "Utilities/Delegates.hpp"

#include "Scene/Entities/Meshes/StaticMesh.hpp"

namespace Engine
{
	DECLARE_MULTICAST_DELEGATE(OnStaticMeshLoaded, AStaticMesh*);

	class SceneDelegates : public Singleton<SceneDelegates>
	{
	public:
		OnStaticMeshLoaded OnStaticMeshLoaded;
	};
}