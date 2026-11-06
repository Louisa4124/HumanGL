#include "Matrix.hpp"
#include "Object.hpp"
#include "Vector.hpp"
#include "internal.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstring>

extern e_state g_state_anim;

Object::Object(void) {};
Object::~Object(void) {};

Object::Object(vec3 scv, vec4 cols, mat4 rootModel) : mother(nullptr), transform(rootModel)
{
    std::memcpy(&scale, &scv, sizeof(vec3));
    std::memcpy(&color, &cols, sizeof(vec4));

    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);

};

Object::Object(vec3 scv, vec4 cols, const Object& mother): mother(&mother)
{
    std::memcpy(&scale, &scv, sizeof(vec3));
    std::memcpy(&color, &cols, sizeof(vec4));

    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);
};

// compute the mvp mat4
mat4 Object::MVP(mat4 pv) { return Matrix::multiply(pv, model); }
