#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <GL/glew.h>
#include <string>

namespace utils
{

    class Geometry
    {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        GLsizei indexCount;

        void createCube();

    public:
        Geometry(const std::string &name);
        ~Geometry();

        Geometry(const Geometry &) = delete;
        Geometry &operator=(const Geometry &) = delete;

        void bind() const;
        void unbind() const;
        GLsizei getIndexCount() const;
    };

}

#endif