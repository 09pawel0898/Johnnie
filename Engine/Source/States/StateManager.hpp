#pragma once

#include "../Core/CoreMinimal.hpp"

#include "State.hpp"

//#include "../Core/Debug.h"

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

			explicit PendingAction(Action Action, StateName StateName = "NONE");
		};

	private:
		std::stack<StatePointer>						m_States;
		std::unordered_map<StateName, StateConstructor>	m_StateConstructors;
		std::vector<PendingAction>						m_PendingActions;
		State::Context									m_Context;

	public:
		explicit StateManager(State::Context Context);

		template <typename StateType>
		void RegisterState(StateName StateName);

		void OnRender(void) const;
		void OnUpdate(double DeltaTime);
		void OnEvent(Events::Event& Event);

		void PushState(StateName StateName);
		void PopState(void);
		bool IsEmpty(void) const;
	};

	template<typename StateType>
	inline void StateManager::RegisterState(StateName StateName)
	{
		//EN_DEBUGMSG("%s%s%s", "[State] State '", stateName.c_str(), "' registered");
		m_StateConstructors[StateName] = [this](void)
		{
			return State::StatePointer(new StateType(*this, m_Context));
		};
	}

	inline void StateManager::PushState(StateName StateName)
	{
		//EN_DEBUGMSG("%s%s%s", "[State] State '", stateName.c_str(), "' pushed");
		m_PendingActions.push_back(PendingAction(Action::ADD, StateName));
	}

	inline void StateManager::PopState(void)
	{
		//EN_DEBUGMSG("%s", "[State] State popped from the top");
		m_PendingActions.push_back(PendingAction(Action::DELETE));
	}

	inline bool StateManager::IsEmpty(void) const
	{
		return m_States.empty();
	}
}