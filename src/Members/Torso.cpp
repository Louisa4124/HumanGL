#include "Members/Torso.hpp"
#include "internal.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

extern e_state g_state_anim;

Torso::Torso() {};
Torso::~Torso() {};

Torso::Torso(vec3 scv, vec4 clv) : Object(scv, clv)
{
};

void Torso::Update(mat4 rMod) {
    model = Matrix::multiply(rMod, scaleMat);
    transform = rMod;
};
