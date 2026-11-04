#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
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
        std::array<float, 16> torsoTransform = modelRoot;
        std::array<float, 16> torsoScale = Matrix::createScale(1.2f, torsoScaleY, 0.6f);
        std::array<float, 16> torsoModel = Matrix::multiply(torsoTransform, torsoScale);
        std::array<float, 16> mvpTorse = Matrix::multiply(Matrix::multiply(projection, view), torsoModel);
        myShader.setMat4("u_MVP", mvpTorse);
        myShader.setVec4("u_Color", 1.0f, 0.5f, 0.2f, 1.0f);

        torso.bind();
        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // la tete : enfant du torse
        float headScaleY = 0.6f;
        float calculatedTranslateY = 1.0f + (headScaleY / 2.0f);
        std::array<float, 16> headTranslate = Matrix::createTranslationY(calculatedTranslateY);
        std::array<float, 16> headScale = Matrix::createScale(0.6f, headScaleY, 0.6f); // mise a lechel : la tete plus petite
        std::array<float, 16> headLocal = Matrix::multiply(headTranslate, headScale);
        std::array<float, 16> headModel = Matrix::multiply(torsoTransform, headLocal);
        std::array<float, 16> mvpHead = Matrix::multiply(Matrix::multiply(projection, view), headModel);
        myShader.setMat4("u_MVP", mvpHead);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // le bras : enfant du torse
        float armScaleX = 0.3f;
        float armScaleY = 0.8f;
        float armScaleZ = 0.3f;
        float armTranslateX = 0.6f + (armScaleX / 2.0f);
        float armTranslateY = (armScaleY / 2.0f);
        std::array<float, 16> armTranslateXMatrix = Matrix::createTranslationX(armTranslateX);
        std::array<float, 16> armTranslateYMatrix = Matrix::createTranslationY(armTranslateY);
        std::array<float, 16> armTranslate = Matrix::multiply(armTranslateYMatrix, armTranslateXMatrix);
        std::array<float, 16> armScale = Matrix::createScale(armScaleX, armScaleY, armScaleZ);
        std::array<float, 16> armRotationX = Matrix::createRotationX(0.0f);
        std::array<float, 16> armScaledAndRotated = Matrix::multiply(armRotationX, armScale);
        std::array<float, 16> armLocal = Matrix::multiply(armTranslate, armScaledAndRotated);
        std::array<float, 16> armModel = Matrix::multiply(torsoTransform, armLocal);
        std::array<float, 16> mvpArm = Matrix::multiply(Matrix::multiply(projection, view), armModel);
        myShader.setMat4("u_MVP", mvpArm);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // l'avant-bras : enfant du bras
        float forearmScaleX = 1.0f;
        float forearmScaleY = 1.0f;
        float forearmScaleZ = 1.0f;
        float forearmTranslateY = -0.5f - (forearmScaleY / 2.0f);
        std::array<float, 16> forearmTranslate = Matrix::createTranslationY(forearmTranslateY);
        std::array<float, 16> forearmScale = Matrix::createScale(forearmScaleX, forearmScaleY, forearmScaleZ);
        std::array<float, 16> forearmRotationX = Matrix::createRotationX(0.0f);
        std::array<float, 16> forearmScaledAndRotated = Matrix::multiply(forearmRotationX, forearmScale);
        std::array<float, 16> forearmLocal = Matrix::multiply(forearmTranslate, forearmScaledAndRotated);
        std::array<float, 16> forearmModel = Matrix::multiply(armModel, forearmLocal);
        std::array<float, 16> mvpForearm = Matrix::multiply(Matrix::multiply(projection, view), forearmModel);
        myShader.setMat4("u_MVP", mvpForearm);
        myShader.setVec4("u_Color", 0.9f, 0.7f, 0.5f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bras : enfant du torse
        float arm2ScaleX = 0.3f;
        float arm2ScaleY = 0.8f;
        float arm2ScaleZ = 0.3f;
        float arm2TranslateX = -0.6f - (arm2ScaleX / 2.0f);
        float arm2TranslateY = (armScaleY / 2.0f);
        std::array<float, 16> arm2TranslateXMatrix = Matrix::createTranslationX(arm2TranslateX);
        std::array<float, 16> arm2TranslateYMatrix = Matrix::createTranslationY(arm2TranslateY);
        std::array<float, 16> arm2Translate = Matrix::multiply(arm2TranslateYMatrix, arm2TranslateXMatrix);
        std::array<float, 16> arm2Scale = Matrix::createScale(arm2ScaleX, arm2ScaleY, arm2ScaleZ);
        std::array<float, 16> arm2RotationX = Matrix::createRotationX(-45.0f);
        std::array<float, 16> arm2ScaledAndRotated = Matrix::multiply(arm2RotationX, arm2Scale);
        std::array<float, 16> arm2Local = Matrix::multiply(arm2Translate, arm2ScaledAndRotated);
        std::array<float, 16> arm2Model = Matrix::multiply(torsoTransform, arm2Local);
        std::array<float, 16> mvpArm2 = Matrix::multiply(Matrix::multiply(projection, view), arm2Model);
        myShader.setMat4("u_MVP", mvpArm2);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e avant-bras : enfant du 2e bras
        float forearm2ScaleX = 1.0f;
        float forearm2ScaleY = 1.0f;
        float forearm2ScaleZ = 1.0f;
        float forearm2TranslateY = -0.5f - (forearm2ScaleY / 2.0f);
        std::array<float, 16> forearm2Translate = Matrix::createTranslationY(forearm2TranslateY);
        std::array<float, 16> forearm2Scale = Matrix::createScale(forearm2ScaleX, forearm2ScaleY, forearm2ScaleZ);
        std::array<float, 16> forearm2RotationX = Matrix::createRotationX(0.0f);
        std::array<float, 16> forearm2ScaledAndRotated = Matrix::multiply(forearm2RotationX, forearm2Scale);
        std::array<float, 16> forearm2Local = Matrix::multiply(forearm2Translate, forearm2ScaledAndRotated);
        std::array<float, 16> forearm2Model = Matrix::multiply(arm2Model, forearm2Local);
        std::array<float, 16> mvpForearm2 = Matrix::multiply(Matrix::multiply(projection, view), forearm2Model);
        myShader.setMat4("u_MVP", mvpForearm2);
        myShader.setVec4("u_Color", 0.9f, 0.7f, 0.5f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        torso.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}