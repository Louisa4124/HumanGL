#ifndef GLFW_H
#define GLFW_H
#include "internal.h"
#include <GLFW/glfw3.h>

rcode       initGlfw(void);
GLFWwindow* createWindow(int width, int height, const char* name);

rcode initGlew();

#endif
