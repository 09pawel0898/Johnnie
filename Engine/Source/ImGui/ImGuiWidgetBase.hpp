#pragma once

#include <functional>
#include <unordered_map>

#include "Core/Debug/Asserts.hpp"

template <typename ActionName>
class ImGuiWidgetBase
{
public:
	using ActionCallback = std::function<void()>;

private:
	struct ActionNameClassHash
	{
		template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	std::unordered_map<ActionName, ActionCallback, ActionNameClassHash> m_Actions;

protected:
	template<typename... Args>
	void CallAction(ActionName Name, Args&&... args)
	{
		const auto existingAction = m_Actions.find(Name);
		CheckMsg(existingAction != m_Actions.cend(), "No delegate is bound to this action.");
		m_Actions[Name](std::forward<decltype(args)>(args)...);
	}

public:
	void BindToAction(ActionName Name, ActionCallback Callback)
	{
		const auto existingAction = m_Actions.find(Name);
		CheckMsg(existingAction == m_Actions.cend(), "Tried to bind already bound action.");
		m_Actions[Name] = Callback;
	}

	virtual void OnRenderGui(void) = 0;
};