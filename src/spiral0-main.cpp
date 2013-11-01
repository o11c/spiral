#define GL_WRAP_GLUT
#include "gl_wrap.hpp"

#include "../config.h"

#include <cmath>
#include <cstdio>

#include <limits>

#include "spine.hpp"
#include "state.hpp"

const float UP_SCALE  = (16.f/15.f);
const float DOWN_SCALE = (15.f/16.f);

static
void inc(float& f, float high)
{
    if (f < high)
        f *= UP_SCALE;
}

static
void dec(float low, float& f)
{
    if (low < f)
        f *= DOWN_SCALE;
}

static
void inc(int& i, int high)
{
    if (i < high)
        i++;
}

static
void dec(int low, int& i)
{
    if (low < i)
        i--;
}

static
void toggle(bool& b)
{
    b = not b;
}


Drawing *root_object = nullptr;
Spine *the_spine = nullptr;

float rho;
float theta, phi;
int sx, sy;

static
void reset()
{
    rho = 500;
    theta = 45 * M_PI/180;
    phi = 45 * M_PI/180;
    the_spine->a = 100;
    the_spine->b = 40;
    the_spine->r = 10;
    the_spine->p = 2;
    the_spine->q = 5;
    the_spine->n = 10;
    the_spine->m = 10;

    the_spine->mesh_rings = the_spine->mesh_longs;
}

static
void drag(int x, int y)
{
    if (x == sx and y == sy)
        return;
    // one degree per pixel should be manageable
    theta -= (x - sx) * M_PI / 180;
    phi -= (y - sy) * M_PI / 180;
    float epsilon = std::numeric_limits<float>::epsilon() * M_PI;
    if (phi < epsilon) phi = epsilon;
    if (phi > M_PI - epsilon) phi = M_PI - epsilon;
    glutPostRedisplay();
    sx = x;
    sy = y;
}

static
void mouse(int button, int state, int x, int y)
{
    // glutMouseWheelFunc does not actually work
    // they are always delivered as button events
    if (button == 3)
    {
        dec(1e1, rho);
        glutPostRedisplay();
        return;
    }
    if (button == 4)
    {
        inc(rho, 1e4);
        glutPostRedisplay();
        return;
    }

    // normal button events
    if (state == GLUT_DOWN)
    {
        sx = x;
        sy = y;
        return;
    }
    drag(x, y);
}

static
void display()
{
    const char *noyes[2] = {"no", "yes"};
    printf("Drawing:\n");
    printf("    spine: %s\n", noyes[the_spine->spine]);
    printf("    mesh_rings: %s\n", noyes[the_spine->mesh_rings]);
    printf("    mesh_longs: %s\n", noyes[the_spine->mesh_longs]);
    printf("    shade: %s\n", noyes[the_spine->shade]);
    printf("    (ρ, θ, φ) = (%f, %f, %f)\n", rho, theta, phi);
    printf("    (a, b, r) = (%f, %f, %f)\n", the_spine->a, the_spine->b, the_spine->r);
    printf("    (p, q) = (%d, %d)\n", the_spine->p, the_spine->q);
    printf("    (n, m) = (%d, %d)\n", the_spine->n, the_spine->m);
    printf("    dirty spine: %s\n", noyes[the_spine->dirty_spine]);
    printf("    dirty mesh: %s\n", noyes[the_spine->dirty_mesh]);
    printf("    shininess: %d\n", encv::materialShininess);
    printf("\n");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    encv::ModelView = mat4();
    encv::ModelView.lookat(
            rho * vec3(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi)),
            {0, 0, 0},
            {0, 0, 1});
    encv::Projection = mat4();
    // no matter what I do, this seems to break down when rho < about 150
    encv::Projection.perspective(40, 1, rho / 50, rho * 2);
    if (root_object)
        root_object->draw();
    glutSwapBuffers();
}

static
void reshape(int w, int h)
{
    if (w > h)
    {
        int d = (w - h) / 2;
        glViewport(d,0, h,h);
    }
    else
    {
        int d = (h - w) / 2;
        glViewport(0,d, w,w);
    }
}

static
void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
    case '\e':
        glutLeaveMainLoop();
        return;
    case '0':
        reset();
        break;
    case '1':
        toggle(the_spine->spine);
        break;
    case '2':
        toggle(the_spine->mesh_rings);
        toggle(the_spine->mesh_longs);
        break;
    case '3':
        toggle(the_spine->shade);
        break;
    case '8':
        toggle(the_spine->mesh_rings);
        break;
    case '9':
        toggle(the_spine->mesh_longs);
        break;
    case 'a': dec(1, the_spine->a); break;
    case 'A': inc(the_spine->a, 1e3); break;
    case 'b': dec(1, the_spine->b); break;
    case 'B': inc(the_spine->b, 1e3); break;
    case 'e': dec(1, encv::materialShininess); break;
    case 'E': inc(encv::materialShininess, 50); break;
    case 'r': dec(1, the_spine->r); break;
    case 'R': inc(the_spine->r, 1e3); break;
    case 'p': dec(1, the_spine->p); break;
    case 'P': inc(the_spine->p, 100); break;
    case 'q': dec(1, the_spine->q); break;
    case 'Q': inc(the_spine->q, 100); break;
    case 'n': dec(3, the_spine->n); break;
    case 'N': inc(the_spine->n, 100); break;
    case 'm': dec(3, the_spine->m); break;
    case 'M': inc(the_spine->m, 100); break;
    default:
        return;
    }
    the_spine->dirty_spine = true;
    the_spine->dirty_mesh = true;
    glutPostRedisplay();
}

static
void init_glut(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Toroidal Spiral");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(drag);
#if HAVE_FREEGLUT
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
            GLUT_ACTION_CONTINUE_EXECUTION);
#else
    printf("You suck!\n");
#endif
}


int main(int argc, char **argv)
{
    init_glut(argc, argv);
    FlatVertexShader fvs;
    ShadeVertexShader svs;
    SimpleFragmentShader sfs;
    BetterFragmentShader bfs;
    FlatProgram fp(&fvs, &sfs);
    ShadeProgram sp(&svs, &bfs);
    Spine spine(&fp, &sp);
    root_object = &spine;
    the_spine = &spine;

    // the donut is reddish, so make the background the opposite
    // this also makes the dark parts of the mesh visible
    glClearColor(0.0, 0.1, 0.1, 1.0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    reset();
    for (int i = 1; i < argc; ++i)
        for (char *a = argv[i]; *a; ++a)
            keyboard(*a, 0, 0);
    glutMainLoop();

    the_spine = nullptr;
    root_object = nullptr;
    puts("Everything is OK");
}
