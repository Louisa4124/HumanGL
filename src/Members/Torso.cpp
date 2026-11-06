#include "Members/Torso.hpp"
#include "internal.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

extern e_state g_state_anim;

Torso::Torso() {};
Torso::~Torso() {};

Torso::Torso(vec3 scv, vec4 clv, mat4 rMod) : Object(scv, clv, rMod)
{
    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);
    localModel = Matrix::multiply(transform, scaleMat);
};

void Torso::Update(void) { model = localModel; };
