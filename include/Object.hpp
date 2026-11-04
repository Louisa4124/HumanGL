#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "Matrix.hpp"
#include "Vector.hpp"

class Object
{
  public:
    vec3 scale;
    vec4 color;
    mat4 transform;
    mat4 model;
    mat4 mvp;

    Object(void);
    Object(vec3 scv, vec4 color, mat4 rootModel, mat4 pv);
    ~Object(void);
};

#endif
