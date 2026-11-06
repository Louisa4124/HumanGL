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
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

const float WIDTH  = 800.0;
const float HEIGHT = 600.0;

e_state g_state_anim = RUNNING;
float   g_angleY      = 10.0;

void draw(Shader& myShader, utils::Geometry& geo, mat4 mvp, vec4 color)
{
    myShader.setMat4("u_MVP", mvp);
    myShader.setVec4("u_Color", color);
    glDrawElements(GL_TRIANGLES, geo.getIndexCount(), GL_UNSIGNED_INT, 0);
}

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
    const mat4 projection = Matrix::createPerspective(45.0, WIDTH / HEIGHT, 0.1, 100.0);
    const mat4 view       = Matrix::createTranslationZ(-7.0);   // on dezoom un peu !
    const mat4 pv         = Matrix::multiply(projection, view);
    const mat4 modelScale = Matrix::createScale(1.0, 1.0, 1.0);

    // scale
    vec3 armScale{ 0.3, 1.1, 0.3 };
    vec3 legScale{ 0.35, 0.9, 0.35 };

    // colors
    vec4 armColor{ 1.0, 0.8, 0.6, 1.0 };
    vec4 foreArmColor{ 0.9, 0.7, 0.5, 1.0 };
    vec4 upLegColor{ 0.2, 0.4, 0.8, 1.0 };
    vec4 lowLegColor{ 0.1, 0.3, 0.7, 1.0 };

    // element
    Torso    torso(vec3{ 1.2, 2.0, 0.8 }, vec4{ 1.0, 0.5, 0.2, 1.0 });
    Head     head(vec3{ 0.7, 0.7, 0.7 }, vec4{ 1.0, 0.8, 0.6, 1.0 }, torso);
    Arm      arm1(armScale, armColor, LEFT, torso);
    ForeArm  foreArm1(arm1.scale, foreArmColor, LEFT, arm1);
    Arm      arm2(armScale, armColor, RIGHT, torso);
    ForeArm  foreArm2(arm2.scale, foreArmColor, RIGHT, arm2);
    UpperLeg upperLeg1(legScale, upLegColor, LEFT, torso);
    LowerLeg lowerLeg1(upperLeg1.scale, lowLegColor, LEFT, upperLeg1);
    UpperLeg upperLeg2(legScale, upLegColor, RIGHT, torso);
    LowerLeg lowerleg2(upperLeg2.scale, lowLegColor, LEFT, upperLeg2);

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
        const mat4 modelRoY = Matrix::createRotationY(g_angleY);
        const mat4 modelRoot
            = Matrix::multiply(jumpTranslation, Matrix::multiply(modelRoY, modelScale));

        // elements
        // le torse
        torso.Update(modelRoot);
        draw(myShader, baseCube, torso.MVP(pv), torso.color);

        // la tete : enfant du torse
        head.Update();
        draw(myShader, baseCube, head.MVP(pv), head.color);

        // le bras : enfant du torse
        arm1.Update();
        draw(myShader, baseCube, arm1.MVP(pv), arm1.color);

        // l'avant-bras : enfant du bras
        foreArm1.Update();
        draw(myShader, baseCube, foreArm1.MVP(pv), foreArm1.color);

        // 2e bras : enfant du torse
        arm2.Update();
        draw(myShader, baseCube, arm2.MVP(pv), arm2.color);

        // 2e avant-bras : enfant du 2e bras
        foreArm2.Update();
        draw(myShader, baseCube, foreArm2.MVP(pv), foreArm2.color);

        // 1ere jambe (haut de jambe) : enfant du torse
        upperLeg1.Update(jumpKneeBend);
        draw(myShader, baseCube, upperLeg1.MVP(pv), upperLeg1.color);

        // 1er bas de jambe : enfant du haut de jambe
        lowerLeg1.Update(jumpKneeBend);
        draw(myShader, baseCube, lowerLeg1.MVP(pv), lowerLeg1.color);

        // 2e jambe (haut de jambe) : enfant du torse
        upperLeg2.Update(jumpKneeBend);
        draw(myShader, baseCube, upperLeg2.MVP(pv), upperLeg2.color);

        // 2e bas de jambe : enfant du 2e haut de jambe
        lowerleg2.Update(jumpKneeBend);
        draw(myShader, baseCube, lowerleg2.MVP(pv), lowerleg2.color);

        baseCube.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
