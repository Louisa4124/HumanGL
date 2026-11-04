#include "Vector.hpp"

std::ostream& operator <<(std::ostream& out, const vec4& v)
{
    out << " {";
    out << v.x << ' ';
    out << v.y << ' ';
    out << v.z << ' ';
    out << v.w << ' ';
    out << " }";

    return out;
}
