#pragma once

#include "Core/CoreMinimal.hpp"
#include "Tickable.hpp"

namespace Engine
{
	using OUUID = std::string;

	class Object : public Tickable
	{
	private:
		std::string m_UUID;
		
	public:
		Object();
		virtual ~Object() = default;

		virtual void OnTick(double DeltaTime) = 0;

		std::string const& GetUUID(void) const;
	};

	FORCEINLINE std::string const& Object::GetUUID(void) const
	{
		return m_UUID;
	}
}