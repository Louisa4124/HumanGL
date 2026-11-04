#include "Object.hpp"

Object::Object(void) {};
Object::~Object(void) {};

Object::Object(vec3 scv, mat4 rootModel)
{
    scaleVec  = scv;
    transform = rootModel;
    scale     = Matrix::createScale(scv.x, scv.y, scv.z);
};
