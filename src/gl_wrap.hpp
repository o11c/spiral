#ifndef GL_WRAP_HPP
#define GL_WRAP_HPP

#include "../config.h"

#if USE_GLES_HACK and not defined(GL_WRAP_GLUT)
# include <GLES2/gl2.h>
#else //GLES2
# define GL_GLEXT_PROTOTYPES
# if defined(__APPLE__) || defined(MACOSX)
#  include <OpenGL/gl.h>
#  include <GLUT/glut.h>
# else
#  include <GL/gl.h>
#  include <GL/glut.h>
# endif
# if HAVE_FREEGLUT
#  include <GL/freeglut.h>
# else
void glutLeaveMainLoop();
# endif
#endif // GLES2

#include <type_traits>

// allow utility headers to not pull in all of OpenGL
static_assert(std::is_same<GLenum, unsigned int>::value, "GLenum != unsigned int");
static_assert(std::is_same<GLint, int>::value, "GLint != int");
static_assert(std::is_same<GLfloat, float>::value, "GLfloat != float");



#endif //GL_WRAP_HPP
