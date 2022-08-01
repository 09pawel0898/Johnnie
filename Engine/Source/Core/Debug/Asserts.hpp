#pragma once

#include <iostream>

#include "Log/Log.hpp"

using namespace Engine;

#ifndef NDEBUG
#   define VERBOSITY "Assert failed"

#   define Check(Expr) \
	    if(!(Expr)) __debugbreak()
#else
#   define VERBOSITY "Abort called"

#   define Check(Expr) \
        if(!(Expr)) abort()
#endif

#   define CheckMsg(Expr, Msg) \
        __CheckMsg(#Expr, Expr, __FILE__, __LINE__, Msg)

inline void __CheckMsg(const char* ExprStr, bool Expr, const char* File, int Line, const char* Msg)
{
    if (!Expr)
    {
        LOG(Core, Error, "\n{0}:\t{1}\nExpected:  {2}\nSource:\t   {3}, line {4}\n", VERBOSITY, Msg, ExprStr, File, Line);
#ifndef NDEBUG
        __debugbreak();
#else
        abort();
#endif
    }
}