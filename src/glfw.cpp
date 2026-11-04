#include "internal.h"
#include <GLFW/glfw3.h>

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

// switch key {
// case glfw.KeyEscape:
// 	loopStatus = true

// // X
// case glfw.KeyW:
// 	cam.Move(camera.Forward, float32(deltaTime))
// case glfw.KeyS:
// 	cam.Move(camera.Backward, deltaTime)

// // Y
// case glfw.KeyD:
// 	cam.Move(camera.Right, deltaTime)
// case glfw.KeyA:
// 	cam.Move(camera.Left, deltaTime)

// // Z
// case glfw.KeyH:
// 	cam.Move(camera.Up, deltaTime)
// case glfw.KeyN:
// 	cam.Move(camera.Down, deltaTime)
// }

// typedef void(* 	GLFWkeyfun) (GLFWwindow *, int, int, int, int)
