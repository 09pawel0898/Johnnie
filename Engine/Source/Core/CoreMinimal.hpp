#pragma once

#include "PlatformCheck.hpp"
#include "Debug/Asserts.hpp"
//#include "Debug/Asserts.hpp"
//#include "Log/Log.hpp"

#include <memory.h>
#include <iostream>

#ifndef NOMINMAX
#define NOMINMAX
#endif

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

/** Should define console log sink by default */
#define PROFILE_RESULT_LOG 0

//#define DEBUG_MODEL_IMPORTER

namespace Engine
{
    struct ID
    {
        static inline const std::string None = "None";
    };

    struct Index
    {
        static inline const int32_t None = -1;
    };
}

template <typename TObject>
using TUniquePtr = std::unique_ptr<TObject>;

template <typename TObject>
using TSharedPtr = std::shared_ptr<TObject>;

template <typename TObject>
using TWeakPtr = std::weak_ptr<TObject>;

template <typename... Args>
FORCEINLINE decltype(auto) MoveTemp(Args&&... Args_)
{
    return std::move(std::forward<Args>(Args_)...);
}

template <typename TForwardType, typename... Args>
FORCEINLINE decltype(auto) Forward(Args&&... Args_)
{
    return std::forward<TForwardType>(std::forward<Args>(Args_)...);
}

template <typename TObject, typename... Args>
FORCEINLINE TUniquePtr<TObject> MakeUnique(Args&&... Args_)
{
    return std::make_unique<TObject>(std::forward<Args>(Args_)...);
}

template <typename TObject, typename... Args>
FORCEINLINE TSharedPtr<TObject> MakeShared(Args&&... Args_)
{
    return std::make_shared<TObject>(std::forward<Args>(Args_)...);
}

template <typename TClass>
using SharedFromThis = std::enable_shared_from_this<TClass>;

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
    return dynamic_cast<const To*>(Ptr);
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
FORCEINLINE constexpr const To* Cast(SmartPtr auto const& Ptr)
{
    return dynamic_cast<const To*>(Ptr.get());
}

#include "System/Memory.hpp"