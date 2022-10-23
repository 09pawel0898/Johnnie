#pragma once

#include <utility>

/** Standard Singleton [constructed automatically] */

template <typename T>
class Singleton
{
    static inline TUniquePtr<T> s_Instance;

public:
    static TUniquePtr<T>& Get()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = MakeUnique<T>();
        }
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

/** Singleton that allows manual object construction */

template <typename T>
class MSingleton
{
    static inline TUniquePtr<T> s_Instance;

protected:
    template <typename... Args>
    static void Construct(Args...)
    {
        s_Instance = MakeUnique<T>(Forward(Args)...);
    }

public:
    static TUniquePtr<T>& Get()
    {
        Check(s_Instance != nullptr);
        return s_Instance;
    }

protected:
    MSingleton() = default;
    virtual ~MSingleton() = default;

    MSingleton(MSingleton const&) = delete;
    MSingleton& operator=(MSingleton const&) = delete;

    MSingleton(MSingleton&&) = delete;
    MSingleton& operator=(MSingleton&&) = delete;
};