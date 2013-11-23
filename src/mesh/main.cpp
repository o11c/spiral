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


Scene *root_scene = nullptr;

float rho;
Radians theta, phi;
int sx, sy;
bool pause_ = true;
vec3 axis;
int view;
vec3 camera_position;
quat camera_orientation;

static
void camera_compat()
{
    camera_position = rho * vec3(sin_(phi) * cos_(theta), sin_(phi) * sin_(theta), cos_(phi));
    camera_orientation =
        quat(Degrees(-90), {0, 0, 1})
        * quat(-phi, {0, 1, 0})
        * quat(-theta, {0, 0, 1});
}

static
void reset()
{
    rho = 5;
    theta = Degrees(45);
    phi = Degrees(45);

    for (auto& model : root_scene->models)
        model.orientation = quat(Degrees(0), {0, 0, 1});
    axis = {0, 0, 1};
    view = 40;

    camera_compat();
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
    camera_compat();
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
        camera_compat();
        glutPostRedisplay();
        return;
    }
    if (button == 4)
    {
        inc(rho, 1e4);
        camera_compat();
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
    printf("    (ρ, θ, φ), fov = (%f, %f, %f), %d\n", rho, theta.value(), phi.value(), view);
    printf("    axis = (%f, %f, %f)%s\n", axis.x, axis.y, axis.z, pause_ ? " (paused)" : "");
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
        ctx.ModelView *= camera_orientation;
        ctx.ModelView.translate(-camera_position);
        ctx.Projection = mat4();
        ctx.Projection.perspective(Degrees(view), 1, rho / 50, rho * 2);
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
    case '-': inc(view, 90); break;
    case '+': dec(1, view); break;
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
        for (auto& ymesh : yscene.multi.meshes)
        {
            PositionedModel mesh;
            mesh.offset = ymesh.position;
            mesh.scale = ymesh.scale;
            mesh.orientation = quat(Degrees(ymesh.orient_angle), ymesh.orient_axis);
            mesh.model = make_unique<Mesh>(&tp, ymesh.mesh);
            scene.models.push_back(std::move(mesh));
        }
        scene.light.position = vec4(yscene.light.position, 1.0);
        scene.light.color = vec4(yscene.light.color, 1.0);
        // TODO change camera control
        (void)yscene.camera;
        (void)yscene.look;
    }
    root_scene = &scene;

    glClearColor(0.0, 0.1, 0.1, 1.0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    checkOpenGLError();
    reset();

    checkOpenGLError();

    glutMainLoop();

    root_scene = nullptr;
    puts("Everything is OK");
}
