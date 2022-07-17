#pragma once

//#include <memory>

#include "../Core/CoreMinimal.hpp"

//#include "../Resources/ResourceManager.h"

namespace Engine::Core
{
	class Window;
}

namespace Engine::Events
{
	class Event;
}

namespace Engine::States
{
	class StateManager;
	class State;

	using StatePointer = std::unique_ptr<State>;

	class State
	{	
	public:
		struct Context
		{
			std::shared_ptr<Core::Window> m_Window;
			//std::shared_ptr<TextureManager> m_Textures;

			explicit Context(std::shared_ptr<Core::Window> Window/*, std::shared_ptr<TextureManager> textures */) noexcept
				:	m_Window(std::move(Window))
					//m_Textures(std::move(textures))
			{}
		};

	private:
		Context			m_Context;
		StateManager*	m_StateManager;

	protected:
		inline Context GetContext(void) const { return m_Context; }

	public:
		State() = delete;
		State(StateManager& StateManager, Context context) noexcept
			:	m_StateManager(&StateManager),
				m_Context(context)
		{}
		virtual ~State(void) = default;

		virtual void OnRender(void) const			= 0;
		virtual bool OnTick(double DeltaTime)		= 0;
		virtual bool OnEvent(Events::Event& Event)	= 0;
		virtual void OnAwake(void) {}
	};
}