#pragma once

#include "Entities/TickableManager.hpp"
#include "Entities/DrawableManager.hpp"
#include "Entities/Lights/LightingManager.hpp"

namespace Engine
{
	class Scene
	{
	private:
		TickableManager m_TickableManager;
		DrawableManager m_DrawableManager;
		LightingManager m_LightingManager;

	public:
		Scene() = default;
		virtual ~Scene() = default;

		Scene(Scene const&) = delete;
		Scene& operator=(Scene const&) = delete;

		Scene(Scene&&) = delete;
		Scene& operator=(Scene&&) = delete;

	public:
		virtual void OnAwake(void) {};
		virtual void OnDetach(void) {};
		virtual void OnTick(double DeltaTime) = 0;

	private:
		void Tick(double DeltaTime);
		void Render(void);
		void RenderImGui(void);

		friend class EngineBaseLayer;

	public:
		TickableManager& GetTickableManager(void)
		{
			return m_TickableManager;
		}

		DrawableManager& GetDrawableManager(void)
		{
			return m_DrawableManager;
		}

		LightingManager& GetLightingManager(void)
		{
			return m_LightingManager;
		}
	};
}