#include "error.hpp"

#include <cstdio>
#include <cstdlib>

#include "gl_wrap.hpp"

static
const char *error_to_string(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:
        return "no error";
    case GL_INVALID_ENUM:
        return "invalid enum";
    case GL_INVALID_VALUE:
        return "invalid value";
    case GL_INVALID_OPERATION:
        return "invalid operation";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "invalid framebuffer operation";
    case GL_OUT_OF_MEMORY:
        return "out of memory";

    // Stacks are only in old GL.
#ifdef GL_STACK_UNDERFLOW
    case GL_STACK_UNDERFLOW:
        return "stack underflow";
#endif
#ifdef GL_STACK_OVERFLOW
    case GL_STACK_OVERFLOW:
        return "stack overflow";
#endif

    default:
        return "unknown error";
    }
}

void checkOpenGLError()
{
    bool wasError = false;
    for (GLenum error = glGetError(); error != GL_NO_ERROR; error = glGetError())
    {
        printf("GL ERROR: %s\n", error_to_string(error));
        wasError = true;
    }
    if (wasError)
        exit(-1);
}

void barf()
{
    checkOpenGLError();
    printf("no GL error, but exiting anyway\n");
    exit(0);
}
