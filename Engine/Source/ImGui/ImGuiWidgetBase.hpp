#pragma once

#include "Core/Debug/Asserts.hpp"
#include "../Scene/Entities/Primitives/Tickable.hpp"

namespace Engine
{
	template <typename Action>
	class ImGuiWidgetBase : public Tickable
	{
	public:
		using SimpleActionDelegate = std::function<void()>;

	private:
		struct ActionClassHash
		{
			template <typename T>
			std::size_t operator()(T t) const
			{
				return static_cast<std::size_t>(t);
			}
		};

		std::unordered_map<Action, SimpleActionDelegate, ActionClassHash> m_SimpleActions;

	protected:
		void ExecuteActionDelegate(Action Action)
		{
			const auto existingAction = m_SimpleActions.find(Action);
			CheckMsg(existingAction != m_SimpleActions.cend(), "No delegate is bound to this action.");
			m_SimpleActions[Action]();
		}

	public:
		void BindActionDelegate(Action Action, SimpleActionDelegate Callback)
		{
			const auto existingAction = m_SimpleActions.find(Action);
			CheckMsg(existingAction == m_SimpleActions.cend(), "Tried to bind already bound action.");
			m_SimpleActions[Action] = Callback;
		}

		virtual void OnRenderGui(void) = 0;
		virtual void OnTick(double DeltaTime) {};
	};
}