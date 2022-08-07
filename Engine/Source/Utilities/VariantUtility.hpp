#pragma once

#include <concepts>

template <typename... Ts>
struct Overloader : Ts...
{
    using Ts::operator()...;
};

template <typename... Ts>
auto MakeInlineVisitor(Ts&&...op)
{
    return Overloader<Ts...>{op...};
}