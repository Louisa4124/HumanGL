#include "Members/UpperLeg.hpp"
#include "internal.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

extern e_state g_state_anim;

UpperLeg::~UpperLeg(void) {};

UpperLeg::UpperLeg(vec3 scv, vec4 clv, e_side side, const Object& mother) :
    Object(scv, clv, mother),
    side(side)
{
    // transform is set by mother.transform
    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);
    localModel = Matrix::multiply(transform, scaleMat);
};

void UpperLeg::Update(float jumpKneeBend)
{
    // translation
    mat4 trY   = Matrix::createTranslationY(-scale.y / 2.0);
    mat4 trMat = trY;

    // rotation
    float swangOff;
    switch (side)
    {
        case LEFT:  swangOff = 3.14159; break;
        case RIGHT: swangOff = 0; break;
    }
    float swang
        = (g_state_anim == RUNNING) ? (std::sin(glfwGetTime() * 5.0 + swangOff) * 30.0) : 0.0;
    float hipAngle = swang - (jumpKneeBend * 0.4);

    mat4 roX   = Matrix::createRotationX(hipAngle);
    mat4 roMat = roX;

    // local model
    localModel = Matrix::multiply(trMat, scaleMat);

    // joint with mother
    float motherTrXOff;
    switch (side)
    {
        case LEFT:  motherTrXOff = 0.3; break;
        case RIGHT: motherTrXOff = -0.3; break;
    }
    mat4 motherTrX   = Matrix::createTranslationX(motherTrXOff);
    mat4 motherTrY   = Matrix::createTranslationY(-(mother->scale.y / 2.0));
    mat4 motherTrMat = Matrix::multiply(motherTrX, motherTrY);

    jointed = Matrix::multiply(mother->transform, Matrix::multiply(motherTrMat, roMat));

    // model
    model = Matrix::multiply(jointed, localModel);
};
