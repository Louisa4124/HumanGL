#include "Color.hpp"
#include "Geometry.hpp"
#include "Matrix.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Vector.hpp"
#include "glfw.hpp"
#include "internal.hpp"
#include "utils.hpp"
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

        float forearmScaleX     = arm1.scale.x;
        float forearmScaleY     = arm1.scale.y;
        float forearmScaleZ     = arm1.scale.z;
        float forearmTranslateY = -forearmScaleY / 2.0f;
        mat4  forearmTranslate  = Matrix::createTranslationY(forearmTranslateY);
        mat4  elbowTranslate    = Matrix::createTranslationY(-arm1.scale.y);
        mat4  elbowJoint        = Matrix::multiply(arm1.jointed, elbowTranslate);
        mat4  forearmScale      = Matrix::createScale(forearmScaleX, forearmScaleY, forearmScaleZ);
        float forearmSwingAngle = (g_state_anim == SIX_SEVENNING)
                                      ? (-70.0f + (std::sin(glfwGetTime() * 10.0f)) * -20.0f)
                                      : -20.0f;
        mat4  forearmRotationX  = Matrix::createRotationX(-20.0f);
        mat4  forearmRotationZ  = Matrix::createRotationX(forearmSwingAngle);
        mat4  forearmRotation
            = (g_state_anim == SIX_SEVENNING) ? forearmRotationZ : forearmRotationX;
        mat4 forearmScaledAndRotated
            = Matrix::multiply(forearmRotation, Matrix::multiply(forearmTranslate, forearmScale));
        mat4 forearmModel = Matrix::multiply(elbowJoint, forearmScaledAndRotated);
        mat4 mvpForearm   = Matrix::multiply(Matrix::multiply(projection, view), forearmModel);
        myShader.setMat4("u_MVP", mvpForearm);
        myShader.setVec4("u_Color", 0.9f, 0.7f, 0.5f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bras : enfant du torse
        float arm2ScaleX           = 0.3f;
        float arm2ScaleY           = 1.1f;
        float arm2ScaleZ           = 0.3f;
        float arm2TranslateX       = -(otto.scale.x / 2.0f) - (arm2ScaleX / 2.0f);
        float arm2TranslateY       = (otto.scale.y / 2.0f);
        mat4  arm2TranslateXMatrix = Matrix::createTranslationX(arm2TranslateX);
        mat4  arm2TranslateYMatrix = Matrix::createTranslationY(arm2TranslateY);
        mat4  arm2Translate        = Matrix::multiply(arm2TranslateYMatrix, arm2TranslateXMatrix);
        mat4  arm2Scale            = Matrix::createScale(arm2ScaleX, arm2ScaleY, arm2ScaleZ);
        mat4  arm2Offset           = Matrix::createTranslationY(-arm2ScaleY / 2.0f);
        float arm2SwingAngle       = (g_state_anim == RUNNING)
                                         ? (std::sin(glfwGetTime() * 5.0f + 3.14159f) * 30.0f)
                                         : 0.0f;
        mat4  arm2RotationX        = Matrix::createRotationX(arm2SwingAngle);
        mat4  arm2RotationZ
            = Matrix::createRotationX((g_state_anim == SIX_SEVENNING) ? -25.0f : 0.0f);
        mat4 arm2Joint = Matrix::multiply(
            otto.transform,
            Matrix::multiply(arm2Translate, Matrix::multiply(arm2RotationZ, arm2RotationX))
        );
        mat4 arm2ScaledAndRotated = Matrix::multiply(arm2Offset, arm2Scale);
        mat4 arm2Model            = Matrix::multiply(arm2Joint, arm2ScaledAndRotated);
        mat4 mvpArm2              = Matrix::multiply(Matrix::multiply(projection, view), arm2Model);
        myShader.setMat4("u_MVP", mvpArm2);
        myShader.setVec4("u_Color", 1.0f, 0.8f, 0.6f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e avant-bras : enfant du 2e bras
        float forearm2ScaleX     = arm2ScaleX;
        float forearm2ScaleY     = arm2ScaleY;
        float forearm2ScaleZ     = arm2ScaleZ;
        float forearm2TranslateY = -forearm2ScaleY / 2.0f;
        mat4  forearm2Translate  = Matrix::createTranslationY(forearm2TranslateY);
        mat4  elbow2Translate    = Matrix::createTranslationY(-arm2ScaleY);
        mat4  elbow2Joint        = Matrix::multiply(arm2Joint, elbow2Translate);
        mat4  forearm2Scale = Matrix::createScale(forearm2ScaleX, forearm2ScaleY, forearm2ScaleZ);
        float forearm2SwingAngle
            = (g_state_anim == SIX_SEVENNING)
                  ? (-70.0f + (std::sin(glfwGetTime() * 10.0f + 3.14159f)) * -20.0f)
                  : -20.0f;
        mat4 forearm2RotationX = Matrix::createRotationX(-20.0f);
        mat4 forearm2RotationZ = Matrix::createRotationX(forearm2SwingAngle);
        mat4 forearm2Rotation
            = (g_state_anim == SIX_SEVENNING) ? forearm2RotationZ : forearm2RotationX;
        mat4 forearm2ScaledAndRotated = Matrix::multiply(
            forearm2Rotation,
            Matrix::multiply(forearm2Translate, forearm2Scale)
        );
        mat4 forearm2Model = Matrix::multiply(elbow2Joint, forearm2ScaledAndRotated);
        mat4 mvpForearm2   = Matrix::multiply(Matrix::multiply(projection, view), forearm2Model);
        myShader.setMat4("u_MVP", mvpForearm2);
        myShader.setVec4("u_Color", 0.9f, 0.7f, 0.5f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 1ere jambe (haut de jambe) : enfant du torse
        float legScaleX           = 0.35f;
        float legScaleY           = 0.9f;
        float legScaleZ           = 0.35f;
        float legTranslateX       = 0.3f;
        float legTranslateY       = -(otto.scale.y / 2.0f);
        mat4  legTranslateXMatrix = Matrix::createTranslationX(legTranslateX);
        mat4  legTranslateYMatrix = Matrix::createTranslationY(legTranslateY);
        mat4  legTranslate        = Matrix::multiply(legTranslateYMatrix, legTranslateXMatrix);
        mat4  legScale            = Matrix::createScale(legScaleX, legScaleY, legScaleZ);
        mat4  legOffset           = Matrix::createTranslationY(-legScaleY / 2.0f);
        float legSwingAngle       = (g_state_anim == RUNNING)
                                        ? (std::sin(glfwGetTime() * 5.0f + 3.14159f) * 30.0f)
                                        : 0.0f;
        float legHipAngle         = legSwingAngle - (jumpKneeBend * 0.4f);
        mat4  legRotationX        = Matrix::createRotationX(legHipAngle);
        mat4  legJoint
            = Matrix::multiply(otto.transform, Matrix::multiply(legTranslate, legRotationX));
        mat4 legScaledAndRotated = Matrix::multiply(legOffset, legScale);
        mat4 legModel            = Matrix::multiply(legJoint, legScaledAndRotated);
        mat4 mvpLeg              = Matrix::multiply(Matrix::multiply(projection, view), legModel);
        myShader.setMat4("u_MVP", mvpLeg);
        myShader.setVec4("u_Color", 0.2f, 0.4f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 1er bas de jambe : enfant du haut de jambe
        float shinScaleX     = legScaleX;
        float shinScaleY     = 1.0f;
        float shinScaleZ     = legScaleZ;
        float shinTranslateY = -shinScaleY / 2.0f;
        mat4  shinTranslate  = Matrix::createTranslationY(shinTranslateY);
        mat4  kneeTranslate  = Matrix::createTranslationY(-legScaleY);
        mat4  kneeJoint      = Matrix::multiply(legJoint, kneeTranslate);
        mat4  shinScale      = Matrix::createScale(shinScaleX, shinScaleY, shinScaleZ);
        mat4  shinRotationX  = Matrix::createRotationX(jumpKneeBend * 0.8f);
        mat4  shinScaledAndRotated
            = Matrix::multiply(shinRotationX, Matrix::multiply(shinTranslate, shinScale));
        mat4 shinModel = Matrix::multiply(kneeJoint, shinScaledAndRotated);
        mat4 mvpShin   = Matrix::multiply(Matrix::multiply(projection, view), shinModel);
        myShader.setMat4("u_MVP", mvpShin);
        myShader.setVec4("u_Color", 0.1f, 0.3f, 0.7f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e jambe (haut de jambe) : enfant du torse
        float leg2ScaleX           = 0.35f;
        float leg2ScaleY           = 0.9f;
        float leg2ScaleZ           = 0.35f;
        float leg2TranslateX       = -0.3f;
        float leg2TranslateY       = -(otto.scale.y / 2.0f);
        mat4  leg2TranslateXMatrix = Matrix::createTranslationX(leg2TranslateX);
        mat4  leg2TranslateYMatrix = Matrix::createTranslationY(leg2TranslateY);
        mat4  leg2Translate        = Matrix::multiply(leg2TranslateYMatrix, leg2TranslateXMatrix);
        mat4  leg2Scale            = Matrix::createScale(leg2ScaleX, leg2ScaleY, leg2ScaleZ);
        mat4  leg2Offset           = Matrix::createTranslationY(-leg2ScaleY / 2.0f);
        float leg2SwingAngle
            = (g_state_anim == RUNNING) ? (std::sin(glfwGetTime() * 5.0f) * 30.0f) : 0.0f;
        float leg2HipAngle  = leg2SwingAngle - (jumpKneeBend * 0.4f);
        mat4  leg2RotationX = Matrix::createRotationX(leg2HipAngle);
        mat4  leg2Joint
            = Matrix::multiply(otto.transform, Matrix::multiply(leg2Translate, leg2RotationX));
        mat4 leg2ScaledAndRotated = Matrix::multiply(leg2Offset, leg2Scale);
        mat4 leg2Model            = Matrix::multiply(leg2Joint, leg2ScaledAndRotated);
        mat4 mvpLeg2              = Matrix::multiply(Matrix::multiply(projection, view), leg2Model);
        myShader.setMat4("u_MVP", mvpLeg2);
        myShader.setVec4("u_Color", 0.2f, 0.4f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, torso.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bas de jambe : enfant du 2e haut de jambe
        float shin2ScaleX     = leg2ScaleX;
        float shin2ScaleY     = 1.0f;
        float shin2ScaleZ     = leg2ScaleZ;
        float shin2TranslateY = -shin2ScaleY / 2.0f;
        mat4  shin2Translate  = Matrix::createTranslationY(shin2TranslateY);
        mat4  knee2Translate  = Matrix::createTranslationY(-leg2ScaleY);
        mat4  knee2Joint      = Matrix::multiply(leg2Joint, knee2Translate);
        mat4  shin2Scale      = Matrix::createScale(shin2ScaleX, shin2ScaleY, shin2ScaleZ);
        mat4  shin2RotationX  = Matrix::createRotationX(jumpKneeBend * 0.8f);
        mat4  shin2ScaledAndRotated
            = Matrix::multiply(shin2RotationX, Matrix::multiply(shin2Translate, shin2Scale));
        mat4 shin2Model = Matrix::multiply(knee2Joint, shin2ScaledAndRotated);
        mat4 mvpShin2   = Matrix::multiply(Matrix::multiply(projection, view), shin2Model);
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
