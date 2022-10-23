#pragma once

#include "Primitives/Tickable.hpp"
#include "Primitives/Object.hpp"

namespace Engine
{

	class TickableManager final
	{
	private:
		
		std::vector<std::pair<OUUID, TWeakPtr<Tickable>>> m_RegisteredTickables;

	public:
		TickableManager() = default;

		TickableManager(TickableManager const&) = delete;
		TickableManager& operator=(TickableManager const&) = delete;

		TickableManager(TickableManager&&) = delete;
		TickableManager& operator=(TickableManager&&) = delete;

	public:
		bool RegisterTickable(TSharedPtr<Object> const& TickableObject);

		void UdateTickableEntities(double DeltaTime);

	private:
		void UnRegisterTickable(OUUID const& ExpiredTickableUUID);
	};
}