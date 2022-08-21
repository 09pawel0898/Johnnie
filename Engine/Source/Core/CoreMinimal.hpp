#pragma once

#include "PlatformCheck.hpp"

#include "Debug/Asserts.hpp"
#include "Log/Log.hpp"

#define BIND_FUNCTION(Func)\
        [this](auto&&... Args) -> decltype(auto) { return this->Func(std::forward<decltype(Args)>(Args)...); }

#define BIND_STATIC_FUNCTION(Func)\
        std::bind(Func);

#define BIND_STATIC_FUNCTION_OneParam(Func)\
        std::bind(Func, std::placeholders::_1);

#define BIND_STATIC_FUNCTION_TwoParams(Func, First, Second)\
        std::bind(Func, std::placeholders::_1, std::placeholders::_2);

#define FORCEINLINE __forceinline

#define __CLASS__ std::remove_reference<decltype(ClassMacro_Impl(this))>::type
template<class T> T& ClassMacro_Impl(const T* t);

#define DEFINE_SUPER(ClassName)\
    protected:\
    using Super = ClassName

/** Should define console log sing by default */
#define PROFILE_RESULT_LOG 1

template<typename Base, typename T>
FORCEINLINE constexpr bool InstanceOf(T const* Ptr)
{
    return dynamic_cast<const Base*>(Ptr) != nullptr;
}

template<typename Base, typename Derived>
FORCEINLINE constexpr bool BaseOf()
{
    return std::is_base_of<Base, Derived>::value;
}

template<typename To, typename From>
FORCEINLINE constexpr To* Cast(From* Ptr)
{
    return dynamic_cast<To*>(Ptr);
}

template<typename To, typename From>
FORCEINLINE constexpr To* Cast(From const* Ptr)
{
    return dynamic_cast<To const*>(Ptr);
}

#include <concepts>

template <typename T>
concept SmartPtr =
requires(T t) 
{
    { t.get() };
};

template<typename To>
FORCEINLINE constexpr To* Cast(SmartPtr auto& Ptr)
{
    return dynamic_cast<To*>(Ptr.get());
}

template<typename To>
FORCEINLINE constexpr To* Cast(SmartPtr auto const& Ptr)
{
    return dynamic_cast<To const*>(Ptr.get());
}

#include "System/Memory.hpp"