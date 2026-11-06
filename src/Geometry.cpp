#include "Geometry.hpp"

#include <iostream>
#include <vector>

namespace utils
{

struct Vertex
{
    float x, y, z;
    float u, v;
};

Geometry::Geometry(const std::string& name) : VAO(0), VBO(0), EBO(0), indexCount(0)
{
    if (name == "cube") createCube();
    else std::cerr << "Erreur: Objet inconnue '" << name << "'\n";
}

Geometry::~Geometry()
{
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

void Geometry::bind() const { glBindVertexArray(VAO); }

void Geometry::unbind() const { glBindVertexArray(0); }

GLsizei Geometry::getIndexCount() const { return indexCount; }

void Geometry::createCube()
{
    std::vector<Vertex> vertices = {
        // avant
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f },
        {  0.5f, -0.5f,  0.5f, 1.0f, 0.0f },
        {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f },
        { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f },

        // arrière
        { -0.5f, -0.5f, -0.5f, 1.0f, 0.0f },
        {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f },
        {  0.5f,  0.5f, -0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f },

        // gauche
        { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f },
        { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f },
        { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f },

        // droite
        {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f },
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f },
        {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f },
        {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f },

        // dessus
        { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f },
        {  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f },

        // dessous
        { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f },
        {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f },
        {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f },
        { -0.5f, -0.5f,  0.5f, 0.0f, 1.0f }
    };

    std::vector<unsigned int> indices
        = { 0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
            12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20 };

    indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

}   // namespace utils
