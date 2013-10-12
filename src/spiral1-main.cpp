#define GL_WRAP_GLUT
#include "gl_wrap.hpp"

#include "../config.h"

#include <cstdio>

#include "spine.hpp"


Drawing *root_object = nullptr;

static
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    encv::ModelView = mat4();
    encv::ModelView.lookat(
            {250, 250, 250},
            {0, 0, 0},
            {0, 0, 1});
    encv::Projection = mat4();
    encv::Projection.perspective(40, 1, 1.0, 1000.0);
    if (root_object)
        root_object->draw();
    glutSwapBuffers();
}

static
void reshape(int w, int h)
{
    glViewport(0,0, w,h);
}

static
void keyboard(unsigned char key, int, int)
{
    if (key == '\e')
        glutLeaveMainLoop();
}

static
void init_glut(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Shaded Glyph");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
#if HAVE_FREEGLUT
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
            GLUT_ACTION_CONTINUE_EXECUTION);
#endif
}


int main(int argc, char **argv)
{
    init_glut(argc, argv);
    FlatVertexShader vs;
    SimpleFragmentShader fs;
    FlatProgram p(&vs, &fs);
    Spine spine(&p);
    root_object = &spine;

    glClearColor(0.5, 0.0, 0.0, 1.0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    root_object = nullptr;
    puts("Everything is OK");
}
