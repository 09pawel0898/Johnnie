#pragma once

#include "Core/CoreMinimal.hpp"

using AUUID = std::string;

namespace Engine::RHI
{
	class RHIResource
	{
	private:
		AUUID m_UUID;

	public:
		RHIResource();
		virtual ~RHIResource() = default;

		AUUID const& GetUUID(void) const;
	};

	FORCEINLINE AUUID const& RHIResource::GetUUID(void) const
	{
		return m_UUID;
	}
}