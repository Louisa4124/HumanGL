#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "Matrix.hpp"
#include "Vector.hpp"

class Object
{
  public:
    vec3 scaleVec;
    vec3 color;
    mat4 transform;
    mat4 scale;
    mat4 model;
    mat4 mvp;

    Object(void);
    Object(vec3 scv, mat4 rootModel);
    ~Object(void);
};

#endif
