#include "internal.hpp"
#include "utils.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern e_state g_state_anim;
extern float   g_angleY;
const float    angleSpeed = 5.0;

rcode initGlfw(void)
{
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (!glfwInit()) return KO;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return OK;
}

void KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) mods;

    static int     six   = GLFW_RELEASE;
    static int     seven = GLFW_RELEASE;
    static e_state last  = g_state_anim;

    switch (key)
    {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, 1); break;
        case GLFW_KEY_1:
            g_state_anim = IDLE;
            last         = g_state_anim;
            break;
        case GLFW_KEY_2:
            g_state_anim = RUNNING;
            last         = g_state_anim;
            break;
        case GLFW_KEY_3:
            g_state_anim = JUMPING;
            last         = g_state_anim;
            break;
        case GLFW_KEY_6:     six = action; break;
        case GLFW_KEY_7:     seven = action; break;
        case GLFW_KEY_LEFT:  g_angleY -= angleSpeed; break;
        case GLFW_KEY_RIGHT: g_angleY += angleSpeed; break;
    }

    if (six != GLFW_RELEASE && seven != GLFW_RELEASE) { g_state_anim = SIX_SEVENNING; }
    else g_state_anim = last;
}

GLFWwindow* createWindow(int width, int height, const char* name)
{
    GLFWwindow* w;

    w = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!w)
    {
        glfwTerminate();
        utils::err("Failed to create window.");

        return NULL;
    }

    glfwMakeContextCurrent(w);
    glfwSetInputMode(w, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(w, KeyHandler);

    return w;
}

rcode initGlew()
{
    glewExperimental = GL_TRUE;
    if (auto r = glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(r) << std::endl;
        return KO;
    }

    return OK;
}
