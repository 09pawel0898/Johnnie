#pragma once

#include <exception>

namespace Engine
{
    class InitializationException : public std::exception
    {
    private:
        const char* m_Msg = "Failed to initialize Engine context.";

    public:
        explicit InitializationException(const char* Msg) noexcept
        {
            m_Msg = Msg;
        }

        const char* what() const noexcept override 
        {
            return m_Msg;
        }
    };
}