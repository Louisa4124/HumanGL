#include "Members/Arm.hpp"
#include "internal.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

extern e_state g_state_anim;

Arm::~Arm(void) {};

Arm::Arm(vec3 scv, vec4 clv, e_side side, const Object& mother) :
    Object(scv, clv, mother),
    side(side)
{
    // transform is set by mother.transform
    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);
    localModel = Matrix::multiply(transform, scaleMat);
};

void Arm::Update(void)
{
    // translation
    mat4 tY   = Matrix::createTranslationY(-scale.y / 2.0);
    mat4 tMat = tY;

    // rotation
    float swangOff;
    switch (side)
    {
        case LEFT:  swangOff = 0; break;
        case RIGHT: swangOff = 3.14159; break;
    }

    float swang
        = (g_state_anim == RUNNING) ? (std::sin(glfwGetTime() * 5.0 + swangOff) * 30.0) : 0.0f;

    mat4 rX   = Matrix::createRotationX(swang);
    mat4 rX67 = Matrix::createRotationX(-25.0);
    mat4 rMat = (g_state_anim == SIX_SEVENNING) ? rX67 : rX;

    // localModel
    mat4 scaledAndRotated = Matrix::multiply(tMat, scaleMat);

    // joint with mother
    float motherTrXOff;

    switch (side)
    {
        case LEFT:  motherTrXOff = (mother->scale.x / 2.0) + (scale.x / 2.0); break;
        case RIGHT: motherTrXOff = -(mother->scale.x / 2.0) - (scale.x / 2.0); break;
    }

    mat4 motherTrX   = Matrix::createTranslationX(motherTrXOff);
    mat4 motherTrY   = Matrix::createTranslationY(mother->scale.y / 2.0);
    mat4 motherTrMat = Matrix::multiply(motherTrY, motherTrX);

    // joint with mother
    jointed = Matrix::multiply(mother->transform, Matrix::multiply(motherTrMat, rMat));

    // global model
    model = Matrix::multiply(jointed, scaledAndRotated);
};
