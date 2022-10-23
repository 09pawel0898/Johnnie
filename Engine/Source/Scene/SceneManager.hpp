#pragma once

#include "Scene.hpp"
#include "Utilities/Singleton.hpp"

namespace Engine
{
	class SceneManager final : public Singleton<SceneManager>
	{
	private:
		TUniquePtr<Scene> m_ActiveScene = nullptr;
		
	public:
		SceneManager() = default;
	
	public:
		void SetScene(TUniquePtr<Scene>&& Scene);

		TUniquePtr<Scene>& GetActiveScene(void);
	};

	FORCEINLINE TUniquePtr<Scene>& SceneManager::GetActiveScene(void)
	{
		return m_ActiveScene;
	}
}