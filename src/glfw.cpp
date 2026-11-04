#include <GLFW/glfw3.h>
#include "internal.h"

rcode initGlfw(void)
{
    if (!glfwInit()) return KO;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return OK;
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
    // glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return w;
}
