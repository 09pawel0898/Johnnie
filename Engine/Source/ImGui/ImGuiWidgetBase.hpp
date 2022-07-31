#pragma once

#include <functional>
#include <unordered_map>

#include "Core/Debug/Asserts.hpp"

template <typename ActionName>
class ImGuiWidgetBase
{
public:
	using SimpleActionDelegate = std::function<void()>;

private:
	struct ActionNameClassHash
	{
		template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	std::unordered_map<ActionName, SimpleActionDelegate, ActionNameClassHash> m_SimpleActions;

protected:

	void Execute(ActionName Name)
	{
		const auto existingAction = m_SimpleActions.find(Name);
		CheckMsg(existingAction != m_SimpleActions.cend(), "No delegate is bound to this action.");
		m_SimpleActions[Name]();
	}

public:
	void BindDelegate(ActionName Name, SimpleActionDelegate Callback)
	{
		const auto existingAction = m_SimpleActions.find(Name);
		CheckMsg(existingAction == m_SimpleActions.cend(), "Tried to bind already bound action.");
		m_SimpleActions[Name] = Callback;
	}

	virtual void OnRenderGui(void) = 0;
};