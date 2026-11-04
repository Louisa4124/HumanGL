#include "internal.h"
#include <GLFW/glfw3.h>

extern int g_state_anim;

rcode initGlfw(void)
{
    if (!glfwInit()) return KO;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return OK;
}

void KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, 1);
        case GLFW_KEY_1:      g_state_anim = IDLE; break;
        case GLFW_KEY_2:      g_state_anim = RUNNING; break;
        case GLFW_KEY_3:      g_state_anim = JUMPING; break;
        case GLFW_KEY_4:      g_state_anim = SIX_SEVENNING; break;
    }
}

GLFWwindow* initWindow(int width, int height, const char* name)
{
    GLFWwindow* w;

    w = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!w)
    {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(w);
    glfwSetInputMode(w, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(w, KeyHandler);

    return w;
}
