#pragma once

#include <iostream>
#include <functional>
#include <utility>
#include <cstdint>
#include <stack>
#include <map>
#include <cassert>
#include <chrono>

#include "Debug/Asserts.hpp"
#include "../Utilities/Log.hpp"

#define BIND_EVENT_FUNCTION(Func)\
        [this](auto&&... Args) -> decltype(auto) { return this->Func(std::forward<decltype(Args)>(Args)...); }
