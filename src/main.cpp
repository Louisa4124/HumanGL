
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "../include/Color.hpp"
#include "../include/Geometry.hpp"
#include "../include/Shader.hpp"
#include "../include/Matrix.hpp"
#include "internal.h"
#include "utils.h"
#include "glfw.h"

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

int main(int argc, char **argv)
{
    // rcode ret;
    GLFWwindow *window ;

    if (initGlfw() != OK) {
        return 1;
    }

    window = initWindow(WIDTH, HEIGHT, "HumanGL");
    if (!window) {
        utils::err("Failed to create window.");
        return 1;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    utils::Geometry torso("cube");

    glEnable(GL_DEPTH_TEST);

    int animation = 2;

    while (!glfwWindowShouldClose(window))
    {
        utils::glClearColorHex(0x000000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();

        // config commune a tout le model
        std::array<float, 16> projection = Matrix::createPerspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);
        std::array<float, 16> view = Matrix::createTranslationZ(-7.0f); // on dezoom un peu !
        float angle = glfwGetTime() * 10.0f;
        float jumpCycle = std::fmod(glfwGetTime(), 2.5f);
        float jumpHeight = 0.0f;
        float jumpKneeBend = 0.0f;

        if (animation == 2) {
            if (jumpCycle < 0.5f)
                jumpKneeBend = 40.0f * (jumpCycle / 0.5f);
            else if (jumpCycle < 0.75f)
                jumpKneeBend = 40.0f * (1.0f - (jumpCycle - 0.5f) / 0.25f);

            if (jumpCycle >= 0.5f && jumpCycle < 1.5f) {
                float jumpProgress = (jumpCycle - 0.5f) / 1.0f;
                jumpHeight = 1.4f * 4.0f * jumpProgress * (1.0f - jumpProgress);
            }
        }

        std::array<float, 16> modelRotation = Matrix::createRotationY(angle);
        std::array<float, 16> modelScale = Matrix::createScale(1.0f, 1.0f, 1.0f);
        std::array<float, 16> jumpTranslation = Matrix::createTranslationY(jumpHeight);
        std::array<float, 16> modelRoot = Matrix::multiply(jumpTranslation, Matrix::multiply(modelRotation, modelScale));

        // le torse
        float torsoScaleX = 1.2f;
        float torsoScaleY = 2.0f;
        float torsoScaleZ = 0.8f;
        std::array<float, 16> torsoTransform = modelRoot;
        std::array<float, 16> torsoScale = Matrix::createScale(torsoScaleX, torsoScaleY, torsoScaleZ);
        std::array<float, 16> torsoModel = Matrix::multiply(torsoTransform, torsoScale);
        std::array<float, 16> mvpTorse = Matrix::multiply(Matrix::multiply(projection, view), torsoModel);
        myShader.setMat4("u_MVP", mvpTorse);
        myShader.setVec4("u_Color", 1.0f, 0.5f, 0.2f, 1.0f);

        torso.bind();
        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // la tete : enfant du torse
        float headScaleX = 0.7f;
        float headScaleY = 0.7f;
        float headScaleZ = 0.7f;
        float calculatedTranslateY = (torsoScaleY / 2) + (headScaleY / 2.0f);
        std::array<float, 16> headTranslate = Matrix::createTranslationY(calculatedTranslateY);
        std::array<float, 16> headScale = Matrix::createScale(headScaleX, headScaleY, headScaleZ); // mise a lechel : la tete plus petite
        std::array<float, 16> headLocal = Matrix::multiply(headTranslate, headScale);
        std::array<float, 16> headModel = Matrix::multiply(torsoTransform, headLocal);
        std::array<float, 16> mvpHead = Matrix::multiply(Matrix::multiply(projection, view), headModel);
        myShader.setMat4("u_MVP", mvpHead);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // le bras : enfant du torse
        float armScaleX = 0.3f;
        float armScaleY = 1.1f;
        float armScaleZ = 0.3f;
        float armTranslateX = (torsoScaleX / 2.0f) + (armScaleX / 2.0f);
        float armTranslateY = (torsoScaleY / 2.0f);
        std::array<float, 16> armTranslateXMatrix = Matrix::createTranslationX(armTranslateX);
        std::array<float, 16> armTranslateYMatrix = Matrix::createTranslationY(armTranslateY);
        std::array<float, 16> armTranslate = Matrix::multiply(armTranslateYMatrix, armTranslateXMatrix);
        std::array<float, 16> armScale = Matrix::createScale(armScaleX, armScaleY, armScaleZ);
        std::array<float, 16> armOffset = Matrix::createTranslationY(-armScaleY / 2.0f);
        float armSwingAngle = (animation == 1) ? (std::sin(glfwGetTime() * 5.0f) * 30.0f) : 0.0f;
        std::array<float, 16> armRotationX = Matrix::createRotationX(armSwingAngle);
        std::array<float, 16> armJoint = Matrix::multiply(torsoTransform, Matrix::multiply(armTranslate, armRotationX));
        std::array<float, 16> armScaledAndRotated = Matrix::multiply(armOffset, armScale);
        std::array<float, 16> armModel = Matrix::multiply(armJoint, armScaledAndRotated);
        std::array<float, 16> mvpArm = Matrix::multiply(Matrix::multiply(projection, view), armModel);
        myShader.setMat4("u_MVP", mvpArm);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // l'avant-bras : enfant du bras
        float forearmScaleX = armScaleX;
        float forearmScaleY = armScaleY;
        float forearmScaleZ = armScaleZ;
        float forearmTranslateY = -forearmScaleY / 2.0f;
        std::array<float, 16> forearmTranslate = Matrix::createTranslationY(forearmTranslateY);
        std::array<float, 16> elbowTranslate = Matrix::createTranslationY(-armScaleY);
        std::array<float, 16> elbowJoint = Matrix::multiply(armJoint, elbowTranslate);
        std::array<float, 16> forearmScale = Matrix::createScale(forearmScaleX, forearmScaleY, forearmScaleZ);
        std::array<float, 16> forearmRotationX = Matrix::createRotationX(-20.0f);
        std::array<float, 16> forearmScaledAndRotated = Matrix::multiply(forearmRotationX, Matrix::multiply(forearmTranslate, forearmScale));
        std::array<float, 16> forearmModel = Matrix::multiply(elbowJoint, forearmScaledAndRotated);
        std::array<float, 16> mvpForearm = Matrix::multiply(Matrix::multiply(projection, view), forearmModel);
        myShader.setMat4("u_MVP", mvpForearm);
        myShader.setVec4("u_Color", 0.9f, 0.7f, 0.5f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bras : enfant du torse
        float arm2ScaleX = 0.3f;
        float arm2ScaleY = 1.1f;
        float arm2ScaleZ = 0.3f;
        float arm2TranslateX = -(torsoScaleX / 2.0f) - (arm2ScaleX / 2.0f);
        float arm2TranslateY = (torsoScaleY / 2.0f);
        std::array<float, 16> arm2TranslateXMatrix = Matrix::createTranslationX(arm2TranslateX);
        std::array<float, 16> arm2TranslateYMatrix = Matrix::createTranslationY(arm2TranslateY);
        std::array<float, 16> arm2Translate = Matrix::multiply(arm2TranslateYMatrix, arm2TranslateXMatrix);
        std::array<float, 16> arm2Scale = Matrix::createScale(arm2ScaleX, arm2ScaleY, arm2ScaleZ);
        std::array<float, 16> arm2Offset = Matrix::createTranslationY(-arm2ScaleY / 2.0f);
        float arm2SwingAngle = (animation == 1) ? (std::sin(glfwGetTime() * 5.0f + 3.14159f) * 30.0f) : 0.0f;
        std::array<float, 16> arm2RotationX = Matrix::createRotationX(arm2SwingAngle);
        std::array<float, 16> arm2Joint = Matrix::multiply(torsoTransform, Matrix::multiply(arm2Translate, arm2RotationX));
        std::array<float, 16> arm2ScaledAndRotated = Matrix::multiply(arm2Offset, arm2Scale);
        std::array<float, 16> arm2Model = Matrix::multiply(arm2Joint, arm2ScaledAndRotated);
        std::array<float, 16> mvpArm2 = Matrix::multiply(Matrix::multiply(projection, view), arm2Model);
        myShader.setMat4("u_MVP", mvpArm2);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e avant-bras : enfant du 2e bras
        float forearm2ScaleX = arm2ScaleX;
        float forearm2ScaleY = arm2ScaleY;
        float forearm2ScaleZ = arm2ScaleZ;
        float forearm2TranslateY = -forearm2ScaleY / 2.0f;
        std::array<float, 16> forearm2Translate = Matrix::createTranslationY(forearm2TranslateY);
        std::array<float, 16> elbow2Translate = Matrix::createTranslationY(-arm2ScaleY);
        std::array<float, 16> elbow2Joint = Matrix::multiply(arm2Joint, elbow2Translate);
        std::array<float, 16> forearm2Scale = Matrix::createScale(forearm2ScaleX, forearm2ScaleY, forearm2ScaleZ);
        std::array<float, 16> forearm2RotationX = Matrix::createRotationX(-20.0f);
        std::array<float, 16> forearm2ScaledAndRotated = Matrix::multiply(forearm2RotationX, Matrix::multiply(forearm2Translate, forearm2Scale));
        std::array<float, 16> forearm2Model = Matrix::multiply(elbow2Joint, forearm2ScaledAndRotated);
        std::array<float, 16> mvpForearm2 = Matrix::multiply(Matrix::multiply(projection, view), forearm2Model);
        myShader.setMat4("u_MVP", mvpForearm2);
        myShader.setVec4("u_Color", 0.9f, 0.7f, 0.5f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 1ere jambe (haut de jambe) : enfant du torse
        float legScaleX = 0.35f;
        float legScaleY = 0.9f;
        float legScaleZ = 0.35f;
        float legTranslateX = 0.3f;
        float legTranslateY = -(torsoScaleY / 2.0f);
        std::array<float, 16> legTranslateXMatrix = Matrix::createTranslationX(legTranslateX);
        std::array<float, 16> legTranslateYMatrix = Matrix::createTranslationY(legTranslateY);
        std::array<float, 16> legTranslate = Matrix::multiply(legTranslateYMatrix, legTranslateXMatrix);
        std::array<float, 16> legScale = Matrix::createScale(legScaleX, legScaleY, legScaleZ);
        std::array<float, 16> legOffset = Matrix::createTranslationY(-legScaleY / 2.0f);
        float legSwingAngle = (animation == 1) ? (std::sin(glfwGetTime() * 5.0f + 3.14159f) * 30.0f) : 0.0f;
        float legHipAngle = legSwingAngle - (jumpKneeBend * 0.4f);
        std::array<float, 16> legRotationX = Matrix::createRotationX(legHipAngle);
        std::array<float, 16> legJoint = Matrix::multiply(torsoTransform, Matrix::multiply(legTranslate, legRotationX));
        std::array<float, 16> legScaledAndRotated = Matrix::multiply(legOffset, legScale);
        std::array<float, 16> legModel = Matrix::multiply(legJoint, legScaledAndRotated);
        std::array<float, 16> mvpLeg = Matrix::multiply(Matrix::multiply(projection, view), legModel);
        myShader.setMat4("u_MVP", mvpLeg);
        myShader.setVec4("u_Color", 0.2f, 0.4f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 1er bas de jambe : enfant du haut de jambe
        float shinScaleX = legScaleX;
        float shinScaleY = 1.0f;
        float shinScaleZ = legScaleZ;
        float shinTranslateY = -shinScaleY / 2.0f;
        std::array<float, 16> shinTranslate = Matrix::createTranslationY(shinTranslateY);
        std::array<float, 16> kneeTranslate = Matrix::createTranslationY(-legScaleY);
        std::array<float, 16> kneeJoint = Matrix::multiply(legJoint, kneeTranslate);
        std::array<float, 16> shinScale = Matrix::createScale(shinScaleX, shinScaleY, shinScaleZ);
        std::array<float, 16> shinRotationX = Matrix::createRotationX(jumpKneeBend * 0.8f);
        std::array<float, 16> shinScaledAndRotated = Matrix::multiply(shinRotationX, Matrix::multiply(shinTranslate, shinScale));
        std::array<float, 16> shinModel = Matrix::multiply(kneeJoint, shinScaledAndRotated);
        std::array<float, 16> mvpShin = Matrix::multiply(Matrix::multiply(projection, view), shinModel);
        myShader.setMat4("u_MVP", mvpShin);
        myShader.setVec4("u_Color", 0.1f, 0.3f, 0.7f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e jambe (haut de jambe) : enfant du torse
        float leg2ScaleX = 0.35f;
        float leg2ScaleY = 0.9f;
        float leg2ScaleZ = 0.35f;
        float leg2TranslateX = -0.3f;
        float leg2TranslateY = -(torsoScaleY / 2.0f);
        std::array<float, 16> leg2TranslateXMatrix = Matrix::createTranslationX(leg2TranslateX);
        std::array<float, 16> leg2TranslateYMatrix = Matrix::createTranslationY(leg2TranslateY);
        std::array<float, 16> leg2Translate = Matrix::multiply(leg2TranslateYMatrix, leg2TranslateXMatrix);
        std::array<float, 16> leg2Scale = Matrix::createScale(leg2ScaleX, leg2ScaleY, leg2ScaleZ);
        std::array<float, 16> leg2Offset = Matrix::createTranslationY(-leg2ScaleY / 2.0f);
        float leg2SwingAngle = (animation == 1) ? (std::sin(glfwGetTime() * 5.0f) * 30.0f) : 0.0f;
        float leg2HipAngle = leg2SwingAngle - (jumpKneeBend * 0.4f);
        std::array<float, 16> leg2RotationX = Matrix::createRotationX(leg2HipAngle);
        std::array<float, 16> leg2Joint = Matrix::multiply(torsoTransform, Matrix::multiply(leg2Translate, leg2RotationX));
        std::array<float, 16> leg2ScaledAndRotated = Matrix::multiply(leg2Offset, leg2Scale);
        std::array<float, 16> leg2Model = Matrix::multiply(leg2Joint, leg2ScaledAndRotated);
        std::array<float, 16> mvpLeg2 = Matrix::multiply(Matrix::multiply(projection, view), leg2Model);
        myShader.setMat4("u_MVP", mvpLeg2);
        myShader.setVec4("u_Color", 0.2f, 0.4f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bas de jambe : enfant du 2e haut de jambe
        float shin2ScaleX = leg2ScaleX;
        float shin2ScaleY = 1.0f;
        float shin2ScaleZ = leg2ScaleZ;
        float shin2TranslateY = -shin2ScaleY / 2.0f;
        std::array<float, 16> shin2Translate = Matrix::createTranslationY(shin2TranslateY);
        std::array<float, 16> knee2Translate = Matrix::createTranslationY(-leg2ScaleY);
        std::array<float, 16> knee2Joint = Matrix::multiply(leg2Joint, knee2Translate);
        std::array<float, 16> shin2Scale = Matrix::createScale(shin2ScaleX, shin2ScaleY, shin2ScaleZ);
        std::array<float, 16> shin2RotationX = Matrix::createRotationX(jumpKneeBend * 0.8f);
        std::array<float, 16> shin2ScaledAndRotated = Matrix::multiply(shin2RotationX, Matrix::multiply(shin2Translate, shin2Scale));
        std::array<float, 16> shin2Model = Matrix::multiply(knee2Joint, shin2ScaledAndRotated);
        std::array<float, 16> mvpShin2 = Matrix::multiply(Matrix::multiply(projection, view), shin2Model);
        myShader.setMat4("u_MVP", mvpShin2);
        myShader.setVec4("u_Color", 0.1f, 0.3f, 0.7f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        torso.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
