#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "Matrix.hpp"
#include "Vector.hpp"

class Object
{
  protected:
    const Object* mother;

    mat4 localModel;

  public:
    vec3 scale;
    vec4 color;
    mat4 transform;
    mat4 scaleMat;
    mat4 jointed;
    mat4 model;
    mat4 mvp;

    Object(void);
    Object(vec3 scv, vec4 color, mat4 rootModel);
    Object(vec3 scv, vec4 color, const Object& mother);
    ~Object(void);

    mat4 MVP(mat4 pv);
};

#endif
