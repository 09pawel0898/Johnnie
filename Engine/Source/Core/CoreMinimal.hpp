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

#define DEFINE_SUPER(ClassName)\
    protected:\
    using Super = ClassName

#include "System/Memory.hpp"