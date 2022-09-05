#include "EnginePCH.hpp"

#include "TickableManager.hpp"

namespace Engine
{
	bool TickableManager::RegisterTickable(std::shared_ptr<Object> const& TickableObject)
	{
		if (!TickableObject->m_bTickableRegistered)
		{
			m_RegisteredTickables.emplace_back(std::make_pair(	TickableObject->GetUUID(),
																std::weak_ptr<Tickable>(TickableObject)));

			TickableObject->m_LastUpdate = Utility::Time::now();
			TickableObject->m_bTickableRegistered = true;
			return true;
		}
		return false;
	}

	void TickableManager::UdateTickableEntities(double DeltaTime)
	{
		std::vector<OUUID> pendingTickablesToUnregister;

		for (auto const& [uuid, tickablePtr] : m_RegisteredTickables)
		{
			if (auto ptr = tickablePtr.lock())
			{
				if (ptr->m_bTickEnabled)
				{
					if (ptr->m_TickInterval != 0.f)
					{
						Utility::TimePoint timeNow = Utility::Time::now();
						Utility::Duration timeElapsedSinceLastUpdate = timeNow - ptr->m_LastUpdate;

						if (timeElapsedSinceLastUpdate.count() > ptr->m_TickInterval)
						{
							ptr->OnTick(DeltaTime);
							ptr->m_LastUpdate = timeNow;
						}
					}
					else
					{
						ptr->OnTick(DeltaTime);
					}
				}
			}
			else
			{
				pendingTickablesToUnregister.emplace_back(uuid);
			}
		}
		std::for_each(pendingTickablesToUnregister.begin(), pendingTickablesToUnregister.end(),
			[this](OUUID const& UUID)
			{
				UnRegisterTickable(UUID);
			});
	}

	void TickableManager::UnRegisterTickable(OUUID const& ExpiredTickableUUID)
	{
		auto const& foundElement = std::find_if(m_RegisteredTickables.cbegin(), m_RegisteredTickables.cend(),
			[&ExpiredTickableUUID](std::pair<OUUID, std::weak_ptr<Tickable>> const& Element)
			{
				return Element.first == ExpiredTickableUUID;
			});

		m_RegisteredTickables.erase(foundElement);
	}
}