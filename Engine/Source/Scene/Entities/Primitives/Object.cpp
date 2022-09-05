#include "EnginePCH.hpp"

#include "Object.hpp"

#define UUID_SYSTEM_GENERATOR
#include <uuid.hpp>

namespace Engine
{
	Object::Object()
	{
		m_UUID = uuids::to_string(uuids::uuid_system_generator{}());
	}
}