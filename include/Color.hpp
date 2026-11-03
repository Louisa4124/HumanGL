#ifndef SCOP_UTILS_COLOR_H
#define SCOP_UTILS_COLOR_H

#include <string>
#include <GL/glew.h>

namespace utils {
    struct Color { float r, g, b, a; };

    // convertit 0xRRGGBB en Color - alpha optionnel
    Color hexToColor(unsigned int hex, float a = 1.0f);

    Color hexStringToColor(const std::string &hexStr, float a = 1.0f);

    inline void glClearColorHex(unsigned int hex, float a = 1.0f) {
        Color c = hexToColor(hex, a);
        glClearColor(c.r, c.g, c.b, c.a);
    }
}

#endif