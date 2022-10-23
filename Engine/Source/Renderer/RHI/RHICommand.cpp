#include "EnginePCH.hpp"

#include "RHICommand.hpp"

namespace Engine::RHI
{
	TUniquePtr<DynamicRHI> RHICommand::s_DynamicRHI = nullptr;
}