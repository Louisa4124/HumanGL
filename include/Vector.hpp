#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <ostream>

typedef struct
{
    float x;
    float y;
} vec2;

typedef struct
{
    float x;
    float y;
    float z;
} vec3;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec4;

std::ostream& operator <<(std::ostream& out, const vec4& v);

#endif
