#include "EnginePCH.hpp"

#include "FileUtility.hpp"

namespace fs = std::filesystem;

namespace Engine::Utility
{
    std::optional<std::string_view> ReadFile(std::string_view FilePath)
    {
        std::ifstream file(FilePath.data());

        if (!file.good())
        {
            return std::nullopt;
        }

        std::stringstream ssBuffer;
        std::string line;

        while (getline(file, line))
        {
            ssBuffer << line << '\n';
        }
        return ssBuffer.str();
    };
}