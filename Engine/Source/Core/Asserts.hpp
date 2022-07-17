#pragma once

#include <iostream>

#ifndef NDEBUG
#   define Check(Expr) \
	    if(!(Expr)) __debugbreak()

#   define CheckMsg(Expr, Msg) \
    __CheckMsg(#Expr, Expr, __FILE__, __LINE__, Msg)

    inline void __CheckMsg(const char* ExprStr, bool Expr, const char* File, int Line, const char* Msg)
    {
        if (!Expr)
        {
            std::cerr << "Assert failed:\t" << Msg << "\n"
                << "Expected:\t" << ExprStr << "\n"
                << "Source:\t\t" << File << ", line " << Line << "\n";
            __debugbreak();
        }
    }
#else
#   define Check(Expr) ;
#   define CheckMsg(Expr, Msg) ;
#endif