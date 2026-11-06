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
void Object::Arm(mat4 mother, vec3 motherScale)
{
    // first translation
    float tX    = (motherScale.x / 2.0f) + (scale.x / 2.0f);
    float tY    = (motherScale.y / 2.0f);
    mat4  tXMat = Matrix::createTranslationX(tX);
    mat4  tYMat = Matrix::createTranslationY(tY);
    mat4  tMat  = Matrix::multiply(tYMat, tXMat);

    // roatation
    float swingAngle = (g_state_anim == RUNNING) ? (std::sin(glfwGetTime() * 5.0f) * 30.0f) : 0.0f;
    mat4  rX         = Matrix::createRotationX(swingAngle);
    mat4  rZ         = Matrix::createRotationX((g_state_anim == SIX_SEVENNING) ? -25.0f : 0.0f);
    mat4  rMat       = Matrix::multiply(rZ, rX);

    // joint with mother
    jointed = Matrix::multiply(mother, Matrix::multiply(tMat, rMat));

    // second translation
    mat4 tY2 = Matrix::createTranslationY(-scale.y / 2.0f);

    // translate again and scale
    mat4 scaledAndRotated = Matrix::multiply(tY2, scaleMat);

    model = Matrix::multiply(jointed, scaledAndRotated);
}

// apply ForeArm-specific transform
void Object::ForeArm(mat4 mother) {}

// apply Leg-specific transform
void Object::Leg(mat4 mother) {}
