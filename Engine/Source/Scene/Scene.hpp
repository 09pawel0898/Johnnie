#pragma once

#include "Entities/TickableManager.hpp"
#include "Entities/DrawableManager.hpp"
#include "Entities/Lights/LightingManager.hpp"
#include "Events/Event.hpp"

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
		virtual void OnAwake(void) = 0;
		virtual void OnDetach(void) = 0;
		virtual void OnTick(double DeltaTime) = 0;

		virtual void OnEvent(Events::Event& Event) {};

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