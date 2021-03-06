#define GL_WRAP_GLUT
#include "../glue/gl_wrap.hpp"

#include "../../config.h"

#include <cmath>
#include <cstdio>

#include <limits>

#include "../bmp.hpp"
#include "../glue/error.hpp"
#include "../math/quat.hpp"
#include "super.hpp"
#include "../state.hpp"
#include "../glue/texture.hpp"

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
Super *the_super = nullptr;

float rho;
Radians theta, phi;
int sx, sy;
bool pause_;

static
void reset()
{
    rho = 5;
    theta = Degrees(45);
    phi = Degrees(45);
    the_super->tor = false;
    the_super->a = true;
    the_super->d = 1.5f;
    the_super->em = 4;
    the_super->en = 4;
    the_super->dn = 32;
    the_super->dm = 32;

    the_super->mesh_rings = the_super->mesh_longs;
}

static
void drag(int x, int y)
{
    if (x == sx and y == sy)
        return;
    // one degree per pixel should be manageable
    theta -= Degrees(x - sx);
    phi -= Degrees(y - sy);
    Radians epsilon = Radians(std::numeric_limits<float>::epsilon() * M_PI);
    if (phi < epsilon) phi = epsilon;
    if (phi > Radians(M_PI) - epsilon) phi = Radians(M_PI) - epsilon;
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
        if (state == GLUT_UP)
            return;
        dec(1e0, rho);
        glutPostRedisplay();
        return;
    }
    if (button == 4)
    {
        if (state == GLUT_UP)
            return;
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
    checkOpenGLError();
    const char *noyes[2] = {"no", "yes"};
    printf("Drawing:\n");
    printf("    mesh_rings: %s\n", noyes[the_super->mesh_rings]);
    printf("    mesh_longs: %s\n", noyes[the_super->mesh_longs]);
    printf("    shade: %s\n", noyes[the_super->shade]);
    printf("    texture: %s\n", noyes[the_super->texture]);
    printf("    (ρ, θ, φ) = (%f, %f, %f)\n", rho, theta.value(), phi.value());
    if (the_super->tor)
        printf("    (d) = (%f)\n", the_super->d);
    printf("    (n, m) = (%f, %f)\n", the_super->en, the_super->em);
    printf("    (N, M) = (%d, %d)\n", the_super->dn, the_super->dm);
    printf("    dirty mesh: %s\n", noyes[the_super->dirty_mesh]);
    printf("    shininess exponent: %d\n", encv::materialShininess);
    printf("\n");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    encv::ModelView = mat4();
    encv::ModelView.lookat(
            rho * vec3(sin_(phi) * cos_(theta), sin_(phi) * sin_(theta), cos_(phi)),
            {0, 0, 0},
            {0, 0, 1});
    encv::Projection = mat4();
    // no matter what I do, this seems to break down when rho < about 150
    encv::Projection.perspective(Degrees(40), 1, rho / 50, rho * 2);
    encv::TextureMatrix = mat4();
    if (the_super->tor)
        encv::TextureMatrix.scale({1, 2, 1});
    if (root_object)
    {
        static int last_time = 0;
        static quat rot = quat(Degrees(0), {0, 0, 1});
        if (pause_)
            last_time = 0;
        else if (last_time == 0)
            last_time = glutGet(GLUT_ELAPSED_TIME);
        else
        {
            int this_time = glutGet(GLUT_ELAPSED_TIME);
            Degrees angle = Degrees((this_time - last_time) / 50.0f);
            last_time = this_time;
            rot *= quat(angle, {0, 0, 1});
            rot.norm();
        }
        encv::View = encv::ModelView;
        SavingMatrix sav(encv::ModelView);
        encv::ModelView *= rot;
        root_object->draw();
    }
    glutSwapBuffers();
    checkOpenGLError();
    if (!pause_)
        glutPostRedisplay();
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
    case ' ':
        toggle(pause_);
        break;
    case '0':
        reset();
        break;
    case '2':
        toggle(the_super->mesh_rings);
        toggle(the_super->mesh_longs);
        break;
    case '3':
        toggle(the_super->shade);
        break;
    case '4':
        toggle(the_super->texture);
        break;
    case '8':
        toggle(the_super->mesh_rings);
        break;
    case '9':
        toggle(the_super->mesh_longs);
        break;
    case 't':
        toggle(the_super->tor);
        break;
    case 'a':
        toggle(the_super->a);
        break;
    case 'e': dec(1, encv::materialShininess); break;
    case 'E': inc(encv::materialShininess, 50); break;
    case 'd': dec(0.1f, the_super->d); break;
    case 'D': inc(the_super->d, 10.f); break;
    case 'p': dec(1, the_super->en); break;
    case 'P': inc(the_super->en, 100); break;
    case 'q': dec(1, the_super->em); break;
    case 'Q': inc(the_super->em, 100); break;
    case 'n': dec(3, the_super->dn); break;
    case 'N': inc(the_super->dn, 100); break;
    case 'm': dec(3, the_super->dm); break;
    case 'M': inc(the_super->dm, 100); break;
    default:
        return;
    }
    the_super->dirty_mesh = true;
    glutPostRedisplay();
}

static
void init_glut(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Superquadric");
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
    TextureVertexShader tvs;
    SimpleFragmentShader sfs;
    BetterFragmentShader bfs;
    TextureFragmentShader tfs;
    FlatProgram fp(&fvs, &sfs);
    ShadeProgram sp(&svs, &bfs);
    TextureProgram tp(&tvs, &tfs);
    Super super(&fp, &sp, &tp);
    root_object = &super;
    the_super = &super;

    checkOpenGLError();
    sampler2D earth_lights(0, Bmp("data/earthlights1k.bmp"));
    sampler2D earth_map(1, Bmp("data/earthmap1k.bmp"));
    sampler2D earth_specular(2, Bmp("data/earthspec1k.bmp"));
    checkOpenGLError();

    encv::ambient_texture = &earth_lights;
    encv::diffuse_texture = &earth_map;
    encv::specular_texture = &earth_specular;

    // the donut is reddish, so make the background the opposite
    // this also makes the dark parts of the mesh visible
    glClearColor(0.0, 0.1, 0.1, 1.0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    checkOpenGLError();
    reset();
    for (int i = 1; i < argc; ++i)
        for (char *a = argv[i]; *a; ++a)
            keyboard(*a, 0, 0);

    checkOpenGLError();

    glutMainLoop();

    the_super = nullptr;
    root_object = nullptr;
    puts("Everything is OK");
}
