#pragma once

#include <type_traits>

template <typename T>
constexpr inline T operator~ (T a)
{
    return static_cast<T>(~static_cast<std::underlying_type_t<T>>(a));
}

template <typename T>
constexpr inline T operator| (T a, T b)
{
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(a) | static_cast<std::underlying_type_t<T>>(b));
}

template <typename T>
constexpr inline T operator& (T a, T b)
{
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(a) & static_cast<std::underlying_type_t<T>>(b));
}

template <typename T>
constexpr inline T operator^ (T a, T b)
{
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(a) ^ static_cast<std::underlying_type_t<T>>(b));
}

template <typename T>
constexpr inline T& operator|= (T& a, T b)
{
    return reinterpret_cast<T&>(reinterpret_cast<std::underlying_type_t<T>&>(a) |= static_cast<std::underlying_type_t<T>>(b));
}

template <typename T>
constexpr inline T& operator&= (T& a, T b)
{
    return reinterpret_cast<T&>(reinterpret_cast<std::underlying_type_t<T>&>(a) &= static_cast<std::underlying_type_t<T>>(b));
}

template <typename T>
constexpr inline T& operator^= (T& a, T b)
{
    return reinterpret_cast<T&>(reinterpret_cast<std::underlying_type_t<T>&>(a) ^= static_cast<std::underlying_type_t<T>>(b));
}