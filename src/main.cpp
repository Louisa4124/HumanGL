#include "../include/Color.hpp"
#include "../include/Geometry.hpp"
#include "../include/Matrix.hpp"
#include "../include/Shader.hpp"
#include "Object.hpp"
#include "Vector.hpp"
#include "glfw.h"
#include "internal.h"
#include "utils.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <cmath>
#include <iostream>

const float WIDTH  = 800.0f;
const float HEIGHT = 600.0f;

e_state g_state_anim = JUMPING;

int main(int argc, char** argv)
{
    GLFWwindow* window;

    if (initGlfw() != OK) { return 1; }
    if (!(window = createWindow(WIDTH, HEIGHT, "HumanGL"))) { return 1; }
    if (initGlew() != OK) { return 1; }

    Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    utils::Geometry torso("cube");

    glEnable(GL_DEPTH_TEST);

    // config commune a tout le model
    const mat4  projection = Matrix::createPerspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);
    const mat4  view       = Matrix::createTranslationZ(-7.0f);   // on dezoom un peu !
    const mat4  pv         = Matrix::multiply(projection, view);
    const float angle      = 10.0f;

    const mat4 modelRotation = Matrix::createRotationY(angle);
    const mat4 modelScale    = Matrix::createScale(1.0, 1.0, 1.0);

    while (!glfwWindowShouldClose(window))
    {
        utils::glClearColorHex(0x000000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();
        torso.bind();

        // jump !!
        const float jumpCycle    = std::fmod(glfwGetTime(), 2.5f);
        float       jumpHeight   = 0.0f;
        float       jumpKneeBend = 0.0f;

        if (g_state_anim == JUMPING)
        {
            if (jumpCycle < 0.5f) jumpKneeBend = 40.0f * (jumpCycle / 0.5f);
            else if (jumpCycle < 0.75f) jumpKneeBend = 40.0f * (1.0f - (jumpCycle - 0.5f) / 0.25f);

            if (jumpCycle >= 0.5f && jumpCycle < 1.5f)
            {
                float jumpProgress = (jumpCycle - 0.5f) / 1.0f;
                jumpHeight         = 1.4f * 4.0f * jumpProgress * (1.0f - jumpProgress);
            }
        }

        const mat4 jumpTranslation = Matrix::createTranslationY(jumpHeight);

        // root model matrice
        const mat4 modelRoot
            = Matrix::multiply(jumpTranslation, Matrix::multiply(modelRotation, modelScale));

        // element
        // le torse
        Object otto(vec3{ 1.2, 2.0, 0.8 }, vec4{ 1.0, 0.5, 0.2, 1.0 }, modelRoot);

        otto.Torso();

        myShader.setMat4("u_MVP", otto.MVP(pv));
        myShader.setVec4("u_Color", otto.color);
        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // la tete : enfant du torse
        vec3 hsc{ 0.7, 0.7, 0.7 };

// NOLINT: should go in head construct before parent cons?
       float calculatedTranslateY = (otto.scale.y / 2) + (hsc.y / 2.0f);
        mat4 headTranslate = Matrix::createTranslationY(calculatedTranslateY);
// NOLINT

        Object head(hsc, vec4{ 1.0, 0.8, 0.6, 1.0 }, headTranslate);

        head.Head(otto.transform);

        myShader.setMat4("u_MVP", head.MVP(pv));
        myShader.setVec4("u_Color", head.color);
        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // le bras : enfant du torse
        Object arm1(vec3{ 0.3, 1.1, 0.3 }, vec4{ 1.0, 0.8, 0.6, 1.0 }, modelRoot);

        arm1.Arm(otto.transform, otto.scale);

        myShader.setMat4("u_MVP", arm1.MVP(pv));
        myShader.setVec4("u_Color", arm1.color);
        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // std::cout << "expect: \n" << mvpArm;
        // std::cout << "got: \n" << arm1.MVP(pv);
        // assert(arm1.MVP(pv) == mvpArm);

        // l'avant-bras : enfant du bras
        // Object forearm1(arm1.scale, vec4{}, modelRoot, pv);

        float                 forearmScaleX     = arm1.scale.x;
        float                 forearmScaleY     = arm1.scale.y;
        float                 forearmScaleZ     = arm1.scale.z;
        float                 forearmTranslateY = -forearmScaleY / 2.0f;
        std::array<float, 16> forearmTranslate  = Matrix::createTranslationY(forearmTranslateY);
        std::array<float, 16> elbowTranslate    = Matrix::createTranslationY(-arm1.scale.y);
        std::array<float, 16> elbowJoint        = Matrix::multiply(arm1.jointed, elbowTranslate);
        std::array<float, 16> forearmScale
            = Matrix::createScale(forearmScaleX, forearmScaleY, forearmScaleZ);
        float forearmSwingAngle = (g_state_anim == SIX_SEVENNING)
                                      ? (-70.0f + (std::sin(glfwGetTime() * 10.0f)) * -20.0f)
                                      : -20.0f;
        std::array<float, 16> forearmRotationX = Matrix::createRotationX(-20.0f);
        std::array<float, 16> forearmRotationZ = Matrix::createRotationX(forearmSwingAngle);
        std::array<float, 16> forearmRotation
            = (g_state_anim == SIX_SEVENNING) ? forearmRotationZ : forearmRotationX;
        std::array<float, 16> forearmScaledAndRotated
            = Matrix::multiply(forearmRotation, Matrix::multiply(forearmTranslate, forearmScale));
        std::array<float, 16> forearmModel = Matrix::multiply(elbowJoint, forearmScaledAndRotated);
        std::array<float, 16> mvpForearm
            = Matrix::multiply(Matrix::multiply(projection, view), forearmModel);
        myShader.setMat4("u_MVP", mvpForearm);
        myShader.setVec4("u_Color", 0.9f, 0.7f, 0.5f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bras : enfant du torse
        float                 arm2ScaleX           = 0.3f;
        float                 arm2ScaleY           = 1.1f;
        float                 arm2ScaleZ           = 0.3f;
        float                 arm2TranslateX       = -(otto.scale.x / 2.0f) - (arm2ScaleX / 2.0f);
        float                 arm2TranslateY       = (otto.scale.y / 2.0f);
        std::array<float, 16> arm2TranslateXMatrix = Matrix::createTranslationX(arm2TranslateX);
        std::array<float, 16> arm2TranslateYMatrix = Matrix::createTranslationY(arm2TranslateY);
        std::array<float, 16> arm2Translate
            = Matrix::multiply(arm2TranslateYMatrix, arm2TranslateXMatrix);
        std::array<float, 16> arm2Scale  = Matrix::createScale(arm2ScaleX, arm2ScaleY, arm2ScaleZ);
        std::array<float, 16> arm2Offset = Matrix::createTranslationY(-arm2ScaleY / 2.0f);
        float arm2SwingAngle             = (g_state_anim == RUNNING)
                                               ? (std::sin(glfwGetTime() * 5.0f + 3.14159f) * 30.0f)
                                               : 0.0f;
        std::array<float, 16> arm2RotationX = Matrix::createRotationX(arm2SwingAngle);
        std::array<float, 16> arm2RotationZ
            = Matrix::createRotationX((g_state_anim == SIX_SEVENNING) ? -25.0f : 0.0f);
        std::array<float, 16> arm2Joint = Matrix::multiply(
            otto.transform,
            Matrix::multiply(arm2Translate, Matrix::multiply(arm2RotationZ, arm2RotationX))
        );
        std::array<float, 16> arm2ScaledAndRotated = Matrix::multiply(arm2Offset, arm2Scale);
        std::array<float, 16> arm2Model = Matrix::multiply(arm2Joint, arm2ScaledAndRotated);
        std::array<float, 16> mvpArm2
            = Matrix::multiply(Matrix::multiply(projection, view), arm2Model);
        myShader.setMat4("u_MVP", mvpArm2);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e avant-bras : enfant du 2e bras
        float                 forearm2ScaleX     = arm2ScaleX;
        float                 forearm2ScaleY     = arm2ScaleY;
        float                 forearm2ScaleZ     = arm2ScaleZ;
        float                 forearm2TranslateY = -forearm2ScaleY / 2.0f;
        std::array<float, 16> forearm2Translate  = Matrix::createTranslationY(forearm2TranslateY);
        std::array<float, 16> elbow2Translate    = Matrix::createTranslationY(-arm2ScaleY);
        std::array<float, 16> elbow2Joint        = Matrix::multiply(arm2Joint, elbow2Translate);
        std::array<float, 16> forearm2Scale
            = Matrix::createScale(forearm2ScaleX, forearm2ScaleY, forearm2ScaleZ);
        float forearm2SwingAngle
            = (g_state_anim == SIX_SEVENNING)
                  ? (-70.0f + (std::sin(glfwGetTime() * 10.0f + 3.14159f)) * -20.0f)
                  : -20.0f;
        std::array<float, 16> forearm2RotationX = Matrix::createRotationX(-20.0f);
        std::array<float, 16> forearm2RotationZ = Matrix::createRotationX(forearm2SwingAngle);
        std::array<float, 16> forearm2Rotation
            = (g_state_anim == SIX_SEVENNING) ? forearm2RotationZ : forearm2RotationX;
        std::array<float, 16> forearm2ScaledAndRotated = Matrix::multiply(
            forearm2Rotation,
            Matrix::multiply(forearm2Translate, forearm2Scale)
        );
        std::array<float, 16> forearm2Model
            = Matrix::multiply(elbow2Joint, forearm2ScaledAndRotated);
        std::array<float, 16> mvpForearm2
            = Matrix::multiply(Matrix::multiply(projection, view), forearm2Model);
        myShader.setMat4("u_MVP", mvpForearm2);
        myShader.setVec4("u_Color", 0.9f, 0.7f, 0.5f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 1ere jambe (haut de jambe) : enfant du torse
        float                 legScaleX           = 0.35f;
        float                 legScaleY           = 0.9f;
        float                 legScaleZ           = 0.35f;
        float                 legTranslateX       = 0.3f;
        float                 legTranslateY       = -(otto.scale.y / 2.0f);
        std::array<float, 16> legTranslateXMatrix = Matrix::createTranslationX(legTranslateX);
        std::array<float, 16> legTranslateYMatrix = Matrix::createTranslationY(legTranslateY);
        std::array<float, 16> legTranslate
            = Matrix::multiply(legTranslateYMatrix, legTranslateXMatrix);
        std::array<float, 16> legScale  = Matrix::createScale(legScaleX, legScaleY, legScaleZ);
        std::array<float, 16> legOffset = Matrix::createTranslationY(-legScaleY / 2.0f);
        float legSwingAngle             = (g_state_anim == RUNNING)
                                              ? (std::sin(glfwGetTime() * 5.0f + 3.14159f) * 30.0f)
                                              : 0.0f;
        float legHipAngle               = legSwingAngle - (jumpKneeBend * 0.4f);
        std::array<float, 16> legRotationX = Matrix::createRotationX(legHipAngle);
        std::array<float, 16> legJoint
            = Matrix::multiply(otto.transform, Matrix::multiply(legTranslate, legRotationX));
        std::array<float, 16> legScaledAndRotated = Matrix::multiply(legOffset, legScale);
        std::array<float, 16> legModel            = Matrix::multiply(legJoint, legScaledAndRotated);
        std::array<float, 16> mvpLeg
            = Matrix::multiply(Matrix::multiply(projection, view), legModel);
        myShader.setMat4("u_MVP", mvpLeg);
        myShader.setVec4("u_Color", 0.2f, 0.4f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 1er bas de jambe : enfant du haut de jambe
        float                 shinScaleX     = legScaleX;
        float                 shinScaleY     = 1.0f;
        float                 shinScaleZ     = legScaleZ;
        float                 shinTranslateY = -shinScaleY / 2.0f;
        std::array<float, 16> shinTranslate  = Matrix::createTranslationY(shinTranslateY);
        std::array<float, 16> kneeTranslate  = Matrix::createTranslationY(-legScaleY);
        std::array<float, 16> kneeJoint      = Matrix::multiply(legJoint, kneeTranslate);
        std::array<float, 16> shinScale = Matrix::createScale(shinScaleX, shinScaleY, shinScaleZ);
        std::array<float, 16> shinRotationX = Matrix::createRotationX(jumpKneeBend * 0.8f);
        std::array<float, 16> shinScaledAndRotated
            = Matrix::multiply(shinRotationX, Matrix::multiply(shinTranslate, shinScale));
        std::array<float, 16> shinModel = Matrix::multiply(kneeJoint, shinScaledAndRotated);
        std::array<float, 16> mvpShin
            = Matrix::multiply(Matrix::multiply(projection, view), shinModel);
        myShader.setMat4("u_MVP", mvpShin);
        myShader.setVec4("u_Color", 0.1f, 0.3f, 0.7f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e jambe (haut de jambe) : enfant du torse
        float                 leg2ScaleX           = 0.35f;
        float                 leg2ScaleY           = 0.9f;
        float                 leg2ScaleZ           = 0.35f;
        float                 leg2TranslateX       = -0.3f;
        float                 leg2TranslateY       = -(otto.scale.y / 2.0f);
        std::array<float, 16> leg2TranslateXMatrix = Matrix::createTranslationX(leg2TranslateX);
        std::array<float, 16> leg2TranslateYMatrix = Matrix::createTranslationY(leg2TranslateY);
        std::array<float, 16> leg2Translate
            = Matrix::multiply(leg2TranslateYMatrix, leg2TranslateXMatrix);
        std::array<float, 16> leg2Scale  = Matrix::createScale(leg2ScaleX, leg2ScaleY, leg2ScaleZ);
        std::array<float, 16> leg2Offset = Matrix::createTranslationY(-leg2ScaleY / 2.0f);
        float                 leg2SwingAngle
            = (g_state_anim == RUNNING) ? (std::sin(glfwGetTime() * 5.0f) * 30.0f) : 0.0f;
        float                 leg2HipAngle  = leg2SwingAngle - (jumpKneeBend * 0.4f);
        std::array<float, 16> leg2RotationX = Matrix::createRotationX(leg2HipAngle);
        std::array<float, 16> leg2Joint
            = Matrix::multiply(otto.transform, Matrix::multiply(leg2Translate, leg2RotationX));
        std::array<float, 16> leg2ScaledAndRotated = Matrix::multiply(leg2Offset, leg2Scale);
        std::array<float, 16> leg2Model = Matrix::multiply(leg2Joint, leg2ScaledAndRotated);
        std::array<float, 16> mvpLeg2
            = Matrix::multiply(Matrix::multiply(projection, view), leg2Model);
        myShader.setMat4("u_MVP", mvpLeg2);
        myShader.setVec4("u_Color", 0.2f, 0.4f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bas de jambe : enfant du 2e haut de jambe
        float                 shin2ScaleX     = leg2ScaleX;
        float                 shin2ScaleY     = 1.0f;
        float                 shin2ScaleZ     = leg2ScaleZ;
        float                 shin2TranslateY = -shin2ScaleY / 2.0f;
        std::array<float, 16> shin2Translate  = Matrix::createTranslationY(shin2TranslateY);
        std::array<float, 16> knee2Translate  = Matrix::createTranslationY(-leg2ScaleY);
        std::array<float, 16> knee2Joint      = Matrix::multiply(leg2Joint, knee2Translate);
        std::array<float, 16> shin2Scale
            = Matrix::createScale(shin2ScaleX, shin2ScaleY, shin2ScaleZ);
        std::array<float, 16> shin2RotationX = Matrix::createRotationX(jumpKneeBend * 0.8f);
        std::array<float, 16> shin2ScaledAndRotated
            = Matrix::multiply(shin2RotationX, Matrix::multiply(shin2Translate, shin2Scale));
        std::array<float, 16> shin2Model = Matrix::multiply(knee2Joint, shin2ScaledAndRotated);
        std::array<float, 16> mvpShin2
            = Matrix::multiply(Matrix::multiply(projection, view), shin2Model);
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
