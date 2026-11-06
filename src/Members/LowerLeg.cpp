#include "Members/LowerLeg.hpp"
#include "internal.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

extern e_state g_state_anim;

LowerLeg::~LowerLeg(void) {};

LowerLeg::LowerLeg(vec3 scv, vec4 clv, e_side side, const Object& mother) :
    Object(scv, clv, mother),
    side(side)
{
    // transform is set by mother.transform
    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);
    localModel = Matrix::multiply(transform, scaleMat);
};

void LowerLeg::Update(void)
{
    // translation

    // rotation
    float swangOff;
    switch (side)
    {
        case LEFT:  swangOff = 0; break;
        case RIGHT: swangOff = 3.14159; break;
    }

    // local model

    // joint with mother

    // model
    model = Matrix::multiply(jointed, localModel);
};
