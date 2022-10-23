#include "EnginePCH.hpp"

#include "Asserts.hpp"
#include "Log/Log.hpp"

using namespace Engine;

void __CheckMsg(const char* ExprStr, bool Expr, const char* File, int Line, const char* Msg)
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