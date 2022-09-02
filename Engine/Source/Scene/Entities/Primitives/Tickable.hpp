#pragma once

#include "Core/CoreMinimal.hpp"

#include "Utilities/Singleton.hpp"
#include "Utilities/TimingUtility.hpp"

namespace Engine
{
	class Tickable
	{
	private:
		float	m_TickInterval = 0.f;
		bool	m_bTickEnabled = true;
		bool	m_bRegistered = false;

		Utility::TimePoint m_LastUpdate;

		friend class TickableManager;

	protected:
		Tickable() = default;
		virtual ~Tickable() = default;

	public:
		virtual void OnTick(double DeltaTime) = 0;

		void SetTickInterval(float TickInterval = 0.f);
		void SetTickEnabled(bool TickEnabled);
	};

	FORCEINLINE void Tickable::SetTickInterval(float TickInterval)
	{
		CheckMsg(TickInterval >= 0.f, "Tick interval must be >= 0.f");
		m_TickInterval = TickInterval;
	}

	FORCEINLINE void Tickable::SetTickEnabled(bool TickEnabled)
	{
		m_bTickEnabled = TickEnabled;
	}

	class TickableManager final : public Singleton<TickableManager>
	{
	private:
		using TickableUUID = std::string;

		std::vector<
			std::pair<TickableUUID,std::weak_ptr<Tickable>>
			> m_RegisteredTickables;

	public:
		TickableManager() = default;

		bool RegisterTickable(std::shared_ptr<Tickable> const& TickableEntity);

		void UdateTickableEntities(double DeltaTime);

	private:
		void UnRegisterTickable(TickableUUID const& ExpiredTickableEntityUUID);
	};
}