#include "EnginePCH.hpp"

#include "SceneManager.hpp"

namespace Engine
{
	void SceneManager::SetScene(TUniquePtr<Scene>&& Scene)
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->OnDetach();
		}
		m_ActiveScene = MoveTemp(Scene);
		m_ActiveScene->OnAwake();
	}
}