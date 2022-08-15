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

        std::stringstream shaderSource;
        std::string line;

        while (getline(file, line))
        {
            shaderSource << line << '\n';
        }
        return shaderSource.str();
    };
}