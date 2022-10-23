#pragma once

#ifndef NDEBUG
#   define UNIMPLEMENTED __debugbreak();

#   define VERBOSITY "Assert failed"

#   define Check(Expr) \
	    if(!(Expr)) __debugbreak()
#else
#   define UNIMPLEMENTED (void)0;

#   define VERBOSITY "Abort called"

#   define Check(Expr) \
        if(!(Expr)) abort()
#endif

#   define CheckMsg(Expr, Msg) \
        __CheckMsg(#Expr, Expr, __FILE__, __LINE__, Msg)

void __CheckMsg(const char* ExprStr, bool Expr, const char* File, int Line, const char* Msg);