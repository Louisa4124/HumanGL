#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

    void use() const;

    GLuint getID() const { return m_id; }

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;

private:
    GLuint m_id;

    std::string readShaderSource(const char *filePath);
    void checkCompileErrors(GLuint shader, const std::string &type);
};

#endif