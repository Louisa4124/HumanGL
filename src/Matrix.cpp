#include "Matrix.hpp"
#include <cmath>
#include <ostream>

std::ostream& operator <<(std::ostream& out, const mat4& v)
{
    if (!v.empty())
    {
        out << "[ " << v.at(0) << ' ' << v.at(1) << ' ' << v.at(2) << ' ' << v.at(3) << std::endl;
        out << "  " << v.at(4) << ' ' << v.at(5) << ' ' << v.at(6) << ' ' << v.at(7) << std::endl;
        out << "  " << v.at(8) << ' ' << v.at(9) << ' ' << v.at(10) << ' ' << v.at(11) << std::endl;
        out << "  " << v.at(12) << ' ' << v.at(13) << ' ' << v.at(14) << ' ' << v.at(15) << ']';
        out << std::endl;
    }
    return out;
}

std::array<float, 16> Matrix::createRotationY(float angleDegrees)
{
    float angleRad = angleDegrees * (static_cast<float>(M_PI) / 180.0f);
    float cosA     = std::cos(angleRad);
    float sinA     = std::sin(angleRad);

    return { cosA, 0.0f, -sinA, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             sinA, 0.0f, cosA,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
}

std::array<float, 16> Matrix::createRotationX(float angleDeg)
{
    const float pi  = 3.14159265358979323846f;
    const float rad = angleDeg * (pi / 180.0f);
    const float c   = std::cos(rad);
    const float s   = std::sin(rad);

    // Colonne-major
    return { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, c, s, 0.0f, 0.0f, -s, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
}

std::array<float, 16> Matrix::createRotationZ(float angleDegrees)
{
    float angleRad = angleDegrees * (static_cast<float>(M_PI) / 180.0f);
    float c        = std::cos(angleRad);
    float s        = std::sin(angleRad);

    // Colonne-major
    return { c, s, 0.0f, 0.0f, -s, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
}

std::array<float, 16> Matrix::createTranslationX(float x)
{
    std::array<float, 16> res = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    res[12]                   = x;
    return res;
}

std::array<float, 16> Matrix::createTranslationY(float y)
{
    std::array<float, 16> res = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    res[13]                   = y;
    return res;
}

std::array<float, 16> Matrix::createTranslationZ(float z)
{
    std::array<float, 16> res = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    res[14]                   = z;
    return res;
}

std::array<float, 16> Matrix::createScale(float x, float y, float z)
{
    return {
        x, 0.0f, 0.0f, 0.0f, 0.0f, y, 0.0f, 0.0f, 0.0f, 0.0f, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
}

std::array<float, 16>
Matrix::multiply(const std::array<float, 16>& a, const std::array<float, 16>& b)
{
    std::array<float, 16> r = { 0.0f };

    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            r[col * 4 + row] = a[0 * 4 + row]
                               * b[col * 4 + 0]
                               + a[1 * 4 + row]
                               * b[col * 4 + 1]
                               + a[2 * 4 + row]
                               * b[col * 4 + 2]
                               + a[3 * 4 + row]
                               * b[col * 4 + 3];
        }
    }

    return r;
}

std::array<float, 16>
Matrix::createPerspective(float fovDegrees, float aspect, float nearPlane, float farPlane)
{
    float                 tanHalfFov = std::tan((fovDegrees * 0.5f) * (M_PI / 180.0f));
    std::array<float, 16> res        = { 0.0f };

    res[0]  = 1.0f / (aspect * tanHalfFov);
    res[5]  = 1.0f / tanHalfFov;
    res[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    res[11] = -1.0f;
    res[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

    return res;
}
