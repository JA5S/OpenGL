#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak()
#ifdef _DEBUG
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))//#x returns string of x
#else
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
