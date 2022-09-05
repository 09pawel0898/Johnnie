#include "EnginePCH.hpp"

#include "SceneManager.hpp"

namespace Engine
{
	void SceneManager::SetScene(std::unique_ptr<Scene>&& Scene)
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->OnDetach();
		}
		m_ActiveScene = std::move(Scene);
		m_ActiveScene->OnAwake();
	}
}