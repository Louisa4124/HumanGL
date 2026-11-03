#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>

class Matrix
{
public:
    // Genere une matrice de rotation 4x4 autour de l'axe Y
    static std::array<float, 16> createRotationY(float angleDeg);
    // Genere une matrice de rotation 4x4 autour de l'axe X
    static std::array<float, 16> createRotationX(float angleDeg);
    // Genere une matrice de rotation 4x4 autour de l'axe Z
    static std::array<float, 16> createRotationZ(float angleDeg);
    
    // Genere une matrice de translation 4x4 sur de l'axe X
    static std::array<float, 16> createTranslationX(float x);
    // Genere une matrice de translation 4x4 sur de l'axe Y
    static std::array<float, 16> createTranslationY(float x);
    // Genere une matrice de translation 4x4 sur de l'axe Z
    static std::array<float, 16> createTranslationZ(float x);

    // matrice de mise a l'echelle
    static std::array<float, 16> createScale(float x, float y, float z);
    
    static std::array<float, 16> multiply(const std::array<float, 16> &a,
        const std::array<float, 16> &b);
        
    static std::array<float, 16> createPerspective( float fovDegrees, 
                                                    float aspect, 
                                                    float nearPlane, 
                                                    float farPlane);
};

#endif