#include "Members/Head.hpp"
#include "internal.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

extern e_state g_state_anim;

Head::Head() {};
Head::~Head() {};

Head::Head(vec3 scv, vec4 clv, const Object& mother) : Object(scv, clv, mother)
{
    float tY        = (mother.scale.y / 2) + (scv.y / 2.0f);
    mat4  transform = Matrix::createTranslationY(tY);

    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);
    localModel = Matrix::multiply(transform, scaleMat);
};

void Head::Update(void) { model = Matrix::multiply(mother->transform, localModel); };
