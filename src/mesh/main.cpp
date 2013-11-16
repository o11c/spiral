#define GL_WRAP_GLUT
#include "../glue/gl_wrap.hpp"

#include "../../config.h"

#include <cmath>
#include <cstdio>

#include <fstream>
#include <limits>

#include "../bmp.hpp"
#include "../yaml/dumb.hpp"
#include "../glue/error.hpp"
#include "../lieu/mem.hpp"
#include "../math/quat.hpp"
#include "mesh.hpp"
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

#if 0
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
#endif

static
void toggle(bool& b)
{
    b = not b;
}


Scene *root_scene = nullptr;

float rho;
Radians theta, phi;
int sx, sy;
bool pause_;
vec3 axis;

static
void reset()
{
    rho = 5;
    theta = Degrees(45);
    phi = Degrees(45);

    for (auto& model : root_scene->models)
        model.orientation = quat(Degrees(0), {0, 0, 1});
    axis = {0, 0, 1};
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
        dec(1e0, rho);
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
    checkOpenGLError();
    printf("Drawing:\n");
    printf("    (ρ, θ, φ) = (%f, %f, %f)\n", rho, theta.value(), phi.value());
    printf("\n");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (root_scene)
    {
        static int last_time = 0;
        if (pause_)
            last_time = 0;
        else if (last_time == 0)
            last_time = glutGet(GLUT_ELAPSED_TIME);
        else
        {
            int this_time = glutGet(GLUT_ELAPSED_TIME);
            Degrees angle = Degrees((this_time - last_time) / 50.0f);
            last_time = this_time;

            quat rot(angle, axis);
            rot.norm();
            for (auto& model : root_scene->models)
            {
                model.orientation *= rot;
                model.orientation.norm();
            }
        }

        Context ctx;
        ctx.ModelView = mat4();
        ctx.ModelView.lookat(
                rho * vec3(sin_(phi) * cos_(theta), sin_(phi) * sin_(theta), cos_(phi)),
                {0, 0, 0},
                {0, 0, 1});
        ctx.Projection = mat4();
        ctx.Projection.perspective(Degrees(40), 1, rho / 50, rho * 2);
        root_scene->draw(ctx);
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
    case 'x':
        axis = {1, 0, 0};
        break;
    case 'y':
        axis = {0, 1, 0};
        break;
    case 'z':
        axis = {0, 0, 1};
        break;
    default:
        return;
    }
    glutPostRedisplay();
}

static
void init_glut(int& argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Meshquadric");
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
    if (argc < 2)
    {
        printf("Usage: %s <mesh-file ...>\n", argv[0]);
        return 1;
    }
    TextureVertexShader tvs;
    TextureFragmentShader tfs;
    NewTextureProgram tp(&tvs, &tfs);

    Scene scene;
    for (int i = 1; i < argc; ++i)
    {
        PositionedModel mesh;
        int o = 2 * i - argc;
        mesh.offset = vec3(o, 0, -o);
        mesh.scale = 1.0f;
        mesh.orientation = quat();
        mesh.model = make_unique<Mesh>(&tp, silly_parse(std::ifstream(argv[i])));
        scene.models.push_back(std::move(mesh));
    }
    scene.light.position = vec4(1, 1, 1, 1.0);
    scene.light.color = vec4(1.0, 1.0, 1.0, 1.0);
    root_scene = &scene;

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

    root_scene = nullptr;
    puts("Everything is OK");
}
