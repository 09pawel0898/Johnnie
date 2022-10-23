#pragma once

#include <utility>

/** Standard Singleton [constructed automatically] */

template <typename T>
class Singleton
{
protected:
    static inline TUniquePtr<T> s_Instance{ nullptr };

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
protected:
    static inline TUniquePtr<T> s_Instance{ nullptr };

    template <typename... Args>
    static void Construct(Args...)
    {
        s_Instance = MakeUnique<T>(Forward(Args)...);
    }

public:
    static TUniquePtr<T>& Get()
    {
        CheckMsg(s_Instance != nullptr, "Singleton not initialized yet.");
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