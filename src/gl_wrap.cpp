#define GL_WRAP_GLUT
#include "gl_wrap.hpp"

#include <cstdio>

#if not HAVE_FREEGLUT
void glutLeaveMainLoop()
{
    puts("emulating glutLeaveMainLoop by exiting");
    exit(0);
}
#endif
