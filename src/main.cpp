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
    vec4 upLegColor{ 0.2f, 0.4f, 0.8f, 1.0f };
    vec4 lowLegColor{ 0.1, 0.3, 0.7, 1.0 };

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
        uLeg1.Update(jumpKneeBend);
        myShader.setMat4("u_MVP", uLeg1.MVP(pv));
        myShader.setVec4("u_Color", uLeg1.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 1er bas de jambe : enfant du haut de jambe
        LowerLeg lleg1(uLeg1.scale, lowLegColor, LEFT, uLeg1);
        lleg1.Update(jumpKneeBend);
        myShader.setMat4("u_MVP", lleg1.MVP(pv));
        myShader.setVec4("u_Color", lleg1.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e jambe (haut de jambe) : enfant du torse
        UpperLeg uLeg2(legSc, upLegColor, RIGHT, objTorso);
        uLeg2.Update(jumpKneeBend);
        myShader.setMat4("u_MVP", uLeg2.MVP(pv));
        myShader.setVec4("u_Color", uLeg2.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        // 2e bas de jambe : enfant du 2e haut de jambe
        LowerLeg lleg2(uLeg2.scale, lowLegColor, LEFT, uLeg2);
        lleg2.Update(jumpKneeBend);
        myShader.setMat4("u_MVP", lleg2.MVP(pv));
        myShader.setVec4("u_Color", lleg2.color);
        glDrawElements(GL_TRIANGLES, baseCube.getIndexCount(), GL_UNSIGNED_INT, 0);

        baseCube.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
