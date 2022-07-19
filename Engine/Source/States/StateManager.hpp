#pragma once

#include "../Core/CoreMinimal.hpp"
#include "State.hpp"

namespace Engine::States
{
	class StateManager
	{
		using StateName			= std::string;
		using StateConstructor	= std::function<StatePointer(void)>;

	private:
		StatePointer CreateState(StateName StateName);
		void ProcessPendingActions(void);

	private:
		enum class Action : uint8_t { ADD, DELETE };

		struct PendingAction
		{
			Action		m_Action;
			StateName	m_StateName;

			explicit PendingAction(Action Action, StateName StateName = "NONE") noexcept;
		};

	private:
		std::stack<StatePointer>						m_States;
		std::unordered_map<StateName, StateConstructor>	m_StateConstructors;
		std::vector<PendingAction>						m_PendingActions;
		State::Context									m_Context;

	public:
		explicit StateManager(State::Context Context) noexcept;
		
		[[nodiscard]]
		static std::unique_ptr<StateManager> Create(State::Context Context);

		template<typename StateType>
			requires std::is_base_of_v<State, StateType>
		void RegisterState(StateName StateName);

		void OnRender(void) const;
		void OnTick(double DeltaTime);
		void OnEvent(Events::Event& Event);

		void PushState(StateName StateName);
		void PopState(void);
		bool IsEmpty(void) const;
	};

	template<typename StateType>
		requires std::is_base_of_v<State,StateType>
	inline void StateManager::RegisterState(StateName StateName)
	{
		LOG(States, Trace, "State '{0}' registered.", StateName.c_str());
		m_StateConstructors[StateName] = [this](void)
		{
			return std::make_unique<StateType>(*this, m_Context);
		};
	}

	inline void StateManager::PushState(StateName StateName)
	{
		LOG(States, Trace, "State '{0}' attached.", StateName.c_str());
		m_PendingActions.push_back(PendingAction(Action::ADD, StateName));
	}

	inline void StateManager::PopState(void)
	{
		LOG(States, Trace, "State popped from detached.");
		m_PendingActions.push_back(PendingAction(Action::DELETE));
	}

	inline bool StateManager::IsEmpty(void) const
	{
		return m_States.empty();
	}
}