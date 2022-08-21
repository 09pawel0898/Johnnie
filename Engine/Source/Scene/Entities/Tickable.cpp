#include "EnginePCH.hpp"

#include "Tickable.hpp"


#define UUID_SYSTEM_GENERATOR
#include <uuid.hpp>

namespace Engine
{
	bool TickableManager::RegisterTickable(std::shared_ptr<Tickable> const& TickableEntity)
	{
		std::string UUID = uuids::to_string(uuids::uuid_system_generator{}());

		if (!TickableEntity->m_bRegistered)
		{
			m_RegisteredTickables.emplace_back(std::make_pair(std::move(UUID), std::weak_ptr<Tickable>(TickableEntity)));
			TickableEntity->m_LastUpdate = std::chrono::high_resolution_clock::now();
			TickableEntity->m_bRegistered = true;

			return true;
		}
		return false;
	}

	void TickableManager::UdateTickableEntities(double DeltaTime)
	{
		std::vector<TickableUUID> pendingTickablesToUnregister;

		for (auto const& [uuid, tickablePtr] : m_RegisteredTickables)
		{
			if (auto ptr = tickablePtr.lock())
			{
				if(ptr->m_bTickEnabled)
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
		[this](TickableUUID const& UUID)
		{
			UnRegisterTickable(UUID);
		});
	}

	void TickableManager::UnRegisterTickable(TickableUUID const& ExpiredTickableEntityUUID)
	{
		auto const& foundElement = std::find_if(m_RegisteredTickables.cbegin(), m_RegisteredTickables.cend(), 
			[&ExpiredTickableEntityUUID](std::pair<TickableUUID,std::weak_ptr<Tickable>> const& Element)
			{
				return Element.first == ExpiredTickableEntityUUID;
			});

		m_RegisteredTickables.erase(foundElement);
	}
}