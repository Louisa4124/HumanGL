#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/Color.hpp"
#include "../include/Geometry.hpp"
#include "../include/Shader.hpp"

int main(int argc, char** argv)
{
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "HumanGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    utils::Geometry myCube("cube");

    while (!glfwWindowShouldClose(window)) {
        utils::glClearColorHex(0x000000);
        glClear(GL_COLOR_BUFFER_BIT);

        myShader.use();

        myShader.setVec4("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

        myCube.bind();
        glDrawElements(GL_TRIANGLES, myCube.getIndexCount(), GL_UNSIGNED_INT, 0);
        myCube.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}