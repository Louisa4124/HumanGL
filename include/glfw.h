#ifndef GLFW_H
#define GLFW_H
#include <GLFW/glfw3.h>
#include "internal.h"

rcode       initGlfw(void);
GLFWwindow* initWindow(int width, int height, const char* name);

#endif
