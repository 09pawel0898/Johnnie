#include "EnginePCH.hpp"

#include "Scene.hpp"

namespace Engine
{
	void Scene::Tick(double DeltaTime)
	{
		m_TickableManager.UdateTickableEntities(DeltaTime);

		OnTick(DeltaTime);
	}

	void Scene::Render(void)
	{
		m_DrawableManager.DrawActors();
	}

	void Scene::RenderImGui(void)
	{
		m_DrawableManager.DrawWidgets();
	}
}