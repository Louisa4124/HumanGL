#include "Color.hpp"
#include "Geometry.hpp"
#include "Matrix.hpp"
#include "Members/Arm.hpp"
#include "Members/ForeArm.hpp"
#include "Members/Head.hpp"
#include "Members/LowerLeg.hpp"
#include "Members/Torso.hpp"
#include "Members/UpperLeg.hpp"
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

e_state g_state_anim = RUNNING;

int main(void)
{
    GLFWwindow* window;

    if (initGlfw() != OK) { return 1; }
    if (!(window = createWindow(WIDTH, HEIGHT, "HumanGL"))) { return 1; }
    if (initGlew() != OK) { return 1; }

    Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    utils::Geometry baseCube("cube");

    glEnable(GL_DEPTH_TEST);

    // config commune a tout le model
    const mat4  projection = Matrix::createPerspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);
    const mat4  view       = Matrix::createTranslationZ(-7.0f);   // on dezoom un peu !
    const mat4  pv         = Matrix::multiply(projection, view);
    const float angle      = 10.0f;

    const mat4 modelRotation = Matrix::createRotationY(angle);
    const mat4 modelScale    = Matrix::createScale(1.0, 1.0, 1.0);

    // scale
    vec3 legSc{ 0.35, 0.9, 0.35 };
    // colors
    vec4 armColor{ 1.0, 0.8, 0.6, 1.0 };
    vec4 foreArmColor{ 0.9, 0.7, 0.5, 1.0 };
    vec4 upLegColor{0.2f, 0.4f, 0.8f, 1.0f};

    while (!glfwWindowShouldClose(window))
    {
        utils::glClearColorHex(0x000000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();
        baseCube.bind();

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

        // std::cout << "expect: \n" << objArm1.model;
        // std::cout << "got: \n" << arm1.model;
        // assert(objArm1.model == arm1.model);

        // elements
        // le torse
        Torso objTorso(vec3{ 1.2, 2.0, 0.8 }, vec4{ 1.0, 0.5, 0.2, 1.0 }, modelRoot);
        objTorso.Update();
        myShader.setMat4("u_MVP", objTorso.MVP(pv));
        myShader.setVec4("u_Color", objTorso.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // la tete : enfant du torse
        Head objHead(vec3{ 0.7, 0.7, 0.7 }, vec4{ 1.0, 0.8, 0.6, 1.0 }, objTorso);
        objHead.Update();
        myShader.setMat4("u_MVP", objHead.MVP(pv));
        myShader.setVec4("u_Color", objHead.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // le bras : enfant du torse
        Arm objArm1(vec3{ 0.3, 1.1, 0.3 }, armColor, LEFT, objTorso);
        objArm1.Update();
        myShader.setMat4("u_MVP", objArm1.MVP(pv));
        myShader.setVec4("u_Color", objArm1.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // l'avant-bras : enfant du bras
        // TODO: use ref for scale
        ForeArm objForeArm1(objArm1.scale, foreArmColor, LEFT, objArm1);
        objForeArm1.Update();
        myShader.setMat4("u_MVP", objForeArm1.MVP(pv));
        myShader.setVec4("u_Color", objForeArm1.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bras : enfant du torse
        Arm objArm2(vec3{ 0.3, 1.1, 0.3 }, armColor, RIGHT, objTorso);
        objArm2.Update();
        myShader.setMat4("u_MVP", objArm2.MVP(pv));
        myShader.setVec4("u_Color", objArm2.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e avant-bras : enfant du 2e bras
        ForeArm objForeArm2(objArm2.scale, foreArmColor, RIGHT, objArm2);
        objForeArm2.Update();
        myShader.setMat4("u_MVP", objForeArm2.MVP(pv));
        myShader.setVec4("u_Color", objForeArm2.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 1ere jambe (haut de jambe) : enfant du torse
        UpperLeg uLeg1(legSc, upLegColor, LEFT, objTorso);
        float    legScaleX           = 0.35f;
        float    legScaleY           = 0.9f;
        float    legScaleZ           = 0.35f;
        float    legTranslateX       = 0.3f;
        float    legTranslateY       = -(objTorso.scale.y / 2.0f);
        mat4     legTranslateXMatrix = Matrix::createTranslationX(legTranslateX);
        mat4     legTranslateYMatrix = Matrix::createTranslationY(legTranslateY);
        mat4     legTranslate        = Matrix::multiply(legTranslateYMatrix, legTranslateXMatrix);
        mat4     legScale            = Matrix::createScale(legScaleX, legScaleY, legScaleZ);
        mat4     legOffset           = Matrix::createTranslationY(-legScaleY / 2.0f);
        float    legSwingAngle       = (g_state_anim == RUNNING)
                                           ? (std::sin(glfwGetTime() * 5.0f + 3.14159f) * 30.0f)
                                           : 0.0f;
        float    legHipAngle         = legSwingAngle - (jumpKneeBend * 0.4f);
        mat4     legRotationX        = Matrix::createRotationX(legHipAngle);
        mat4     legJoint
            = Matrix::multiply(objTorso.transform, Matrix::multiply(legTranslate, legRotationX));
        mat4 legScaledAndRotated = Matrix::multiply(legOffset, legScale);
        mat4 legModel            = Matrix::multiply(legJoint, legScaledAndRotated);
        mat4 mvpLeg              = Matrix::multiply(Matrix::multiply(projection, view), legModel);
        myShader.setMat4("u_MVP", mvpLeg);
        myShader.setVec4("u_Color", 0.2f, 0.4f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

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

        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e jambe (haut de jambe) : enfant du torse
        float leg2ScaleX           = 0.35f;
        float leg2ScaleY           = 0.9f;
        float leg2ScaleZ           = 0.35f;
        float leg2TranslateX       = -0.3f;
        float leg2TranslateY       = -(objTorso.scale.y / 2.0f);
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
            = Matrix::multiply(objTorso.transform, Matrix::multiply(leg2Translate, leg2RotationX));
        mat4 leg2ScaledAndRotated = Matrix::multiply(leg2Offset, leg2Scale);
        mat4 leg2Model            = Matrix::multiply(leg2Joint, leg2ScaledAndRotated);
        mat4 mvpLeg2              = Matrix::multiply(Matrix::multiply(projection, view), leg2Model);
        myShader.setMat4("u_MVP", mvpLeg2);
        myShader.setVec4("u_Color", 0.2f, 0.4f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

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

        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        baseCube.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
