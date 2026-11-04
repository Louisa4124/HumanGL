#include "Object.hpp"
#include "Vector.hpp"
#include <cstring>

Object::Object(void) {};
Object::~Object(void) {};

Object::Object(vec3 scv, vec4 cols, mat4 rootModel, mat4 pv)
{
    std::memcpy(&scale, &scv, sizeof(vec3));
    std::memcpy(&color, &cols, sizeof(vec4));
    std::memcpy(&transform, &rootModel, sizeof(mat4));
    // transform = rootModel;

    model     = Matrix::multiply(transform, Matrix::createScale(scv.x, scv.y, scv.z));
    mvp       = Matrix::multiply(pv, model);
};
