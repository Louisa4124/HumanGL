#include "../include/Color.hpp"

#include <algorithm>
#include <stdexcept>

namespace utils
{

Color hexToColor(unsigned int hex, float a)
{
    Color c;
    c.r = ((hex >> 16) & 0xff) / 255.0f;
    c.g = ((hex >> 8) & 0xff) / 255.0f;
    c.b = (hex & 0xff) / 255.0f;
    c.a = a;
    return c;
}

Color hexStringToColor(const std::string& hexStr, float a)
{
    std::string s = hexStr;
    if (!s.empty() && s[0] == '#') s.erase(0, 1);
    if (s.size() != 6) throw std::invalid_argument("hex string must be 6 hex digits");
    unsigned int hex = std::stoul(s, nullptr, 16);
    return hexToColor(hex, a);
}
}
