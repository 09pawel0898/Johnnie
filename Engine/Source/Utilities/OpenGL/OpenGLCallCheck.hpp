#pragma once

/** TO BE REMOVED (no explicit glad includes despite RHI) */
#include <glad/glad.h>

#include "Utilities/Log.hpp"
#include "Core/Debug/Asserts.hpp"

inline void GLClearError(void) 
{
    while (glGetError() != GL_NO_ERROR) {};
}

inline bool GLLogCall(const char* Func, const char* File, int Line)
{
    GLenum error;

    while ((error = glGetError() != GL_NO_ERROR))
    {
        LOG(OpenGL, Trace, "Error at {0} in {1} line {2}", Func, File, Line);
        return false;
    }
    return true;
}

#define GLCall(Expr) GLClearError();\
    Expr;\
    Check(GLLogCall(#Expr, __FILE__, __LINE__))
