#include "FileUtility.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

namespace Engine::Utility
{
    std::optional<std::string_view> ReadFile(std::string_view FilePath)
    {
        std::cout << "Current path is: " << fs::current_path()<< '\n';
        std::cout << "Current root path is: " << fs::current_path().root_path() << '\n';
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