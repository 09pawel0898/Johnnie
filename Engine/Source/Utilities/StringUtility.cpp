#include "StringUtility.hpp"

#include <sstream>
#include <iomanip>
#include <bit>


namespace Engine::Utilities
{
	std::string BytesToString(uint64_t Bytes)
	{
		static constexpr double GB = 1024 * 1024 * 1024;
		static constexpr double MB = 1024 * 1024;
		static constexpr double KB = 1024;

		std::ostringstream result;

		auto GetVerbosity = [](auto const& Bytes) -> std::pair<double, std::string_view>
		{
			if (Bytes > GB)
			{
				return std::make_pair((Bytes / GB), " GB");
			}
			else if (Bytes > MB)
			{
				return std::make_pair((Bytes / MB), " MB");
			}
			else if (Bytes > KB)
			{
				return std::make_pair((Bytes / KB), " KB");
			}
			else
			{
				return std::make_pair(static_cast<float>(Bytes), " bytes");
			}
		};

		auto [number, postfix] = GetVerbosity(Bytes);
		result << std::fixed << std::setprecision(2) << number << postfix;

		return result.str();
	}
}