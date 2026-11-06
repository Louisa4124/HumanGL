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
    scale.y = 1.0;

    // transform is set by mother.transform
    scaleMat   = Matrix::createScale(scale.x, scale.y, scale.z);
    localModel = Matrix::multiply(transform, scaleMat);
};

void LowerLeg::Update(float kneeBend)
{
    // translation
    mat4 trY   = Matrix::createTranslationY(-scale.y / 2.0);
    mat4 trMat = trY;

    // rotation
    mat4 roX   = Matrix::createRotationX(kneeBend * 0.8);
    mat4 roMat = roX;

    // local model
    localModel = Matrix::multiply(roMat, Matrix::multiply(trMat, scaleMat));

    // joint with mother (knee)
    mat4 motherTrMAt = Matrix::createTranslationY(-mother->scale.y);

    jointed = Matrix::multiply(mother->jointed, motherTrMAt);

    // model
    model = Matrix::multiply(jointed, localModel);
};
