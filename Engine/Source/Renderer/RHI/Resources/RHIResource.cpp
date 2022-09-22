#include "EnginePCH.hpp"

#include "RHIResource.hpp"

#define UUID_SYSTEM_GENERATOR
#include <uuid.hpp>

namespace Engine::RHI
{
	RHIResource::RHIResource()
	{
		m_UUID = uuids::to_string(uuids::uuid_system_generator{}());
	}
}