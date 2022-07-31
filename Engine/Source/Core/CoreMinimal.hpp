#pragma once

#include "PlatformCheck.hpp"

#include "Debug/Asserts.hpp"
#include "Utilities/Log.hpp"

#define BIND_EVENT_FUNCTION(Func)\
        [this](auto&&... Args) -> decltype(auto) { return this->Func(std::forward<decltype(Args)>(Args)...); }

#define FORCEINLINE __forceinline

#define DEFINE_SUPER(ClassName)\
    protected:\
    using Super = ClassName

#include "System/Memory.hpp"