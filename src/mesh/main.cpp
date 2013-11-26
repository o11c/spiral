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
#include "mirror.hpp"
#include "../glue/texture.hpp"

#if 0
const float UP_SCALE  = (16.f/15.f);
const float DOWN_SCALE = (15.f/16.f);

static
void inc(float& f, float high)
{
    float v = f * UP_SCALE;
    if (v < high)
        f = v;
    else
        f = high;
}

static
void dec(float low, float& f)
{
    float v = f * DOWN_SCALE;
    if (low < v)
        f = v;
    else
        f = low;
}
#endif

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


Scene *root_scene = nullptr;

int sx, sy;
int view;
vec3 camera_position;
quat camera_orientation;

static
vec3 backward()
{
    return (mat4(-camera_orientation) * vec4{0, 0, 1, 1}).xyz;
}

static
vec3 up()
{
    return (mat4(-camera_orientation) * vec4{0, 1, 0, 1}).xyz;
}

static
vec3 right()
{
    return (mat4(-camera_orientation) * vec4{1, 0, 0, 1}).xyz;
}

static
void drag(int x, int y)
{
    if (x == sx and y == sy)
        return;
    // one degree per pixel should be manageable
    auto theta = Degrees(x - sx);
    auto phi = Degrees(y - sy);
    camera_orientation *= quat(theta, up());
    camera_orientation *= quat(phi, right());
    camera_orientation.norm();
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
        camera_position -= backward();
        glutPostRedisplay();
        return;
    }
    if (button == 4)
    {
        if (state == GLUT_UP)
            return;
        camera_position += backward();
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
    auto c = camera_position;
    auto r = right();
    auto u = up();
    auto b = backward();
    printf("Drawing:\n");
    printf("    (x, y, z), fov = (%f, %f, %f), %d\n", c.x, c.y, c.z, view);
    printf("    right = (%f, %f, %f)\n", r.x, r.y, r.z);
    printf("    up = (%f, %f, %f)\n", u.x, u.y, u.z);
    printf("    backward = (%f, %f, %f)\n", b.x, b.y, b.z);
    printf("\n");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (root_scene)
    {
        Context ctx;
        ctx.ModelView = mat4();
        ctx.ModelView *= camera_orientation;
        ctx.ModelView.translate(-camera_position);
        ctx.Projection = mat4();
        ctx.Projection.perspective(Degrees(view), 1, 0.01, 100);
        root_scene->draw_scene(ctx);
    }
    glutSwapBuffers();
    checkOpenGLError();
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
    case '-': inc(view, 90); break;
    case '+': dec(1, view); break;
    case 'h': camera_position -= right(); break;
    case 'l': camera_position += right(); break;
    case 'j': camera_position -= up(); break;
    case 'k': camera_position += up(); break;
    case 'u': camera_orientation *= quat(Degrees(-1), backward()); break;
    case 'i': camera_orientation *= quat(Degrees(1), backward()); break;
    case 'x': camera_position.x--; break;
    case 'X': camera_position.x++; break;
    case 'y': camera_position.y--; break;
    case 'Y': camera_position.y++; break;
    case 'z': camera_position.z--; break;
    case 'Z': camera_position.z++; break;
    default:
        return;
    }
    glutPostRedisplay();
}

static
void init_glut(int& argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("Scene");
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
    if (argc != 2)
    {
        printf("Usage: %s <scene-file>\n", argv[0]);
        return 1;
    }

    TextureVertexShader tvs;
    TextureFragmentShader tfs;
    NewTextureProgram tp(&tvs, &tfs);

    Scene scene;
    {
        YamlScene yscene = parse_scene(std::ifstream(argv[1]));
        auto mirror = make_unique<Mirror>(&tp, yscene.mirror.onto, make_unique<Multi>(&tp, yscene.mirror.multi));
        scene.model = std::move(mirror);
        scene.light.position = vec4(yscene.light.position, 1.0);
        scene.light.color = vec4(yscene.light.color, 1.0);
        // TODO change camera control
        camera_position = yscene.camera;
        vec3 delta = yscene.look - yscene.camera;
        // It looks like I'm getting the arguments of atan2 backwards,
        // but that's the way the math turns out.
        Radians theta(atan2f(delta.x, delta.y));
        Radians phi(atan2f(hypotf(delta.x, delta.y), -delta.z));

        camera_orientation = quat(-phi, {1, 0, 0}) * quat(-theta, {0, 0, -1});
    }
    root_scene = &scene;

    glClearColor(0.0, 0.1, 0.1, 1.0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    checkOpenGLError();
    view = 40;

    checkOpenGLError();

    glutMainLoop();

    root_scene = nullptr;
    puts("Everything is OK");
}
