#include "EnginePCH.hpp"

#include "RHICommand.hpp"

namespace Engine::RHI
{
	std::unique_ptr<DynamicRHI> RHICommand::s_DynamicRHI = nullptr;
}