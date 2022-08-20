#pragma once

#include <utility>

template <typename T>
class Singleton
{
    static inline std::unique_ptr<T> s_Instance;

protected:
    template <typename... Args>
    static void Construct(Args...)
    {
        s_Instance = std::make_unique<T>(std::forward(Args)...);
    }

public:
    static std::unique_ptr<T>& Get()
    {
        Check(s_Instance != nullptr);
        return s_Instance;
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;

    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};