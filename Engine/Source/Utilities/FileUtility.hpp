#pragma once

#include <optional>
#include <string>

namespace Engine::Utility
{
	std::optional<std::string_view> ReadFile(std::string_view FilePath);
}