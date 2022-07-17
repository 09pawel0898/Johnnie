#include "StateManager.hpp"

#include "../Events/Event.hpp"

namespace Engine::States
{
	StateManager::StateManager(State::Context Context) 
		:	m_States(),	
			m_StateConstructors(),
			m_PendingActions(),
			m_Context(Context)
	{}

	std::unique_ptr<StateManager> StateManager::Create(State::Context Context)
	{
		return std::make_unique<StateManager>(Context);
	}

	StatePointer StateManager::CreateState(StateName StateName)
	{
		auto stateConstructor = m_StateConstructors.find(StateName);
		CheckMsg(stateConstructor != m_StateConstructors.end(),"Constructor for requested state was not found.");
		return stateConstructor->second();
	}

	void StateManager::ProcessPendingActions(void)
	{
		for (auto const& pendingAction : m_PendingActions)
		{
			if (pendingAction.m_Action == Action::ADD)
			{
				m_States.push(CreateState(pendingAction.m_StateName));
			}
			else if (pendingAction.m_Action == Action::DELETE)
			{
				m_States.pop();
			}
			m_States.top()->OnAwake();
		}
		m_PendingActions.clear();
	}

	void StateManager::OnRender(void) const
	{
		if (!IsEmpty())
		{
			m_States.top()->OnRender();
		}
	}

	void StateManager::OnTick(double DeltaTime)
	{
		if (!IsEmpty())
		{
			m_States.top()->OnTick(DeltaTime);
		}
		ProcessPendingActions();
	}

	void StateManager::OnEvent(Events::Event& Event)
	{
		if (!IsEmpty())
		{
			m_States.top()->OnEvent(Event);
		}
		ProcessPendingActions();
	}

	StateManager::PendingAction::PendingAction(Action Action, StateName StateName)
		:	m_Action(Action),
			m_StateName(StateName)
	{}
}