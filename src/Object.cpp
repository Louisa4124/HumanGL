#include "Matrix.hpp"
#include "Object.hpp"
#include "Vector.hpp"
#include <cstring>

Object::Object(void) {};
Object::~Object(void) {};

Object::Object(vec3 scv, vec4 cols, mat4 rootModel)
{
    std::memcpy(&scale, &scv, sizeof(vec3));
    std::memcpy(&color, &cols, sizeof(vec4));
    transform = rootModel;

    //  local
    scaleMat   = Matrix::createScale(scv.x, scv.y, scv.z);
    localModel = Matrix::multiply(transform, scaleMat);

    // this is needed for parent, they doesnot apply another transform so model isnt created
    model = localModel;
};

// compute the mvp mat4
mat4 Object::MVP(mat4 pv) { return Matrix::multiply(pv, model); }

// apply torso-specific transform
void Object::Torso(void) { model = localModel; }

// apply Head-specific transform
void Object::Head(mat4 mother) { model = Matrix::multiply(mother, localModel); }

// apply Arm-specific transform
void Object::Arm(mat4 mother) {}

// apply ForeArm-specific transform
void Object::ForeArm(mat4 mother) {}

// apply Leg-specific transform
void Object::Leg(mat4 mother) {}
