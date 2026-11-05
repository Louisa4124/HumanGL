#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "Matrix.hpp"
#include "Vector.hpp"

class Object
{
  private:
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
    ~Object(void);

    mat4 MVP(mat4 pv);

    void Torso(void);
    void Head(mat4 mother);
    void Arm(mat4 mother, vec3 motherScale);
    void ForeArm(mat4 mother);
    void Leg(mat4 mother);
};

#endif
