#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/Color.hpp"
#include "../include/Geometry.hpp"
#include "../include/Shader.hpp"
#include "../include/Matrix.hpp"

int main(int argc, char **argv)
{
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "HumanGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    utils::Geometry torso("cube");
    
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        utils::glClearColorHex(0x000000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();

        // config commune a tout le model
        std::array<float, 16> projection = Matrix::createPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
        std::array<float, 16> view = Matrix::createTranslationZ(-7.0f); // on dezoom un peu !
        float angle = glfwGetTime() * 10.0f;
        std::array<float, 16> modelRotation = Matrix::createRotationY(angle);
        std::array<float, 16> modelScale = Matrix::createScale(1.0f, 1.0f, 1.0f);
        std::array<float, 16> modelRoot = Matrix::multiply(modelRotation, modelScale);

        // le torse
        float torsoScaleY = 2.0f;
        std::array<float, 16> torsoScale = Matrix::createScale(1.2f, torsoScaleY, 0.6f);
        std::array<float, 16> torsoModel = Matrix::multiply(modelRoot, torsoScale); 
        std::array<float, 16> mvpTorse = Matrix::multiply(Matrix::multiply(projection, view), torsoModel);
        myShader.setMat4("u_MVP", mvpTorse);
        myShader.setVec4("u_Color", 1.0f, 0.5f, 0.2f, 1.0f);

        torso.bind();
        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // la tete : enfant du torse
        float headScaleY = 0.4f;
        float calculatedTranslateY = (1.0f / torsoScaleY) + (headScaleY / 2.0f);
        std::array<float, 16> headTranslate = Matrix::createTranslationY(calculatedTranslateY); 
        std::array<float, 16> headScale = Matrix::createScale(0.6f, headScaleY, 0.6f);// mise a lechel : la tete plus petite  
        std::array<float, 16> headLocal = Matrix::multiply(headTranslate, headScale);
        std::array<float, 16> headModel = Matrix::multiply(torsoModel, headLocal);
        std::array<float, 16> mvpHead = Matrix::multiply(Matrix::multiply(projection, view), headModel);
        myShader.setMat4("u_MVP", mvpHead);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // le bras : enfant du torse
        float armScaleX = 0.3f;
        float armScaleY = 0.5f;
        float armScaleZ = 0.3f;
        float armTranslateX = 0.5f + (armScaleX / 2.0f);
        float armTranslateY = 0.5f - (armScaleY / 2.0f); 
        std::array<float, 16> armTranslateXMatrix = Matrix::createTranslationX(armTranslateX);
        std::array<float, 16> armTranslateYMatrix = Matrix::createTranslationY(armTranslateY);
        std::array<float, 16> armTranslate = Matrix::multiply(armTranslateYMatrix, armTranslateXMatrix);
        std::array<float, 16> armScale = Matrix::createScale(armScaleX, armScaleY, armScaleZ);
        std::array<float, 16> armLocal = Matrix::multiply(armTranslate, armScale);
        std::array<float, 16> armModel = Matrix::multiply(torsoModel, armLocal);
        std::array<float, 16> mvpArm = Matrix::multiply(Matrix::multiply(projection, view), armModel);
        myShader.setMat4("u_MVP", mvpArm);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);
        torso.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}