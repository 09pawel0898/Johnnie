#pragma once

#include "Scene.hpp"
#include "Utilities/Singleton.hpp"

namespace Engine
{
	class SceneManager final : public Singleton<SceneManager>
	{
	private:
		std::unique_ptr<Scene> m_ActiveScene = nullptr;
		
	public:
		SceneManager() = default;
	
	public:
		void SetScene(std::unique_ptr<Scene>&& Scene);

		std::unique_ptr<Scene>& GetActiveScene(void);
	};

	FORCEINLINE std::unique_ptr<Scene>& SceneManager::GetActiveScene(void)
	{
		return m_ActiveScene;
	}
}