#pragma once

#include "Core/CoreMinimal.hpp"
#include "Utilities/TimingUtility.hpp"

namespace Engine
{
	class Tickable
	{
	private:
		float	m_TickInterval = 0.f;
		bool	m_bTickEnabled = true;
		bool	m_bTickableRegistered = false;

		Utility::TimePoint m_LastUpdate;

		friend class TickableManager;

	public:
		Tickable() = default;
		virtual ~Tickable() = default;

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
}