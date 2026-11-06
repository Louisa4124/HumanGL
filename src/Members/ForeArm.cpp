#include "Members/ForeArm.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

extern e_state g_state_anim;

ForeArm::~ForeArm(void) {};

ForeArm::ForeArm(vec3 scv, vec4 clv, e_side side, const Object& mother) :
    Object(scv, clv, mother),
    side(side)
{
    // transform is set by mother.transform
    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);
    localModel = Matrix::multiply(transform, scaleMat);
};

void ForeArm::Update(void)
{
    // translation
    mat4 trY = Matrix::createTranslationY(-scale.y / 2.0);
    mat4 trMat = trY;

    // rotation
    float swangOff;
    switch (side)
    {
        case LEFT:  swangOff = 0; break;
        case RIGHT: swangOff = 3.14159; break;
    }

    float swang = (g_state_anim == SIX_SEVENNING)
                      ? (-70.0f + (std::sin(glfwGetTime() * 10.0f + swangOff)) * -20.0f)
                      : -20.0f;

    mat4 roX = Matrix::createRotationX(-20.0f);
    mat4 roX67 = Matrix::createRotationX(swang);
    mat4 roMat = (g_state_anim == SIX_SEVENNING) ? roX67 : roX;

    // local model
    localModel =
        Matrix::multiply(roMat, Matrix::multiply(trMat, scaleMat));

    // joint with mother (elbow)
    mat4 motherTrMat = Matrix::createTranslationY(-mother->scale.y);

    jointed = Matrix::multiply(mother->jointed, motherTrMat);

    // model
    model = Matrix::multiply(jointed, localModel);
};
