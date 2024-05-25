#pragma once

#include <unordered_map>
#include "defs.h"

class Shader {
private:
    unsigned int id = -1;

public:
    Shader(const std::string &vertexFile, const std::string &fragmentFile);
    Shader() {}
    void init(const std::string &vertexFile, const std::string &fragmentFile);
    ~Shader();

    void bind() const;
    void unbind() const;
    
    GLuint u(const char* variableName);
    GLuint a(const char* variableName);

private:
    unsigned int createShaderSource(unsigned int shaderType, const std::string &shaderSource);
    static void setShaderSource(unsigned int shaderId, const std::string &shaderSource);
    static bool compileShader(unsigned int shaderId);
    static unsigned int createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);
    static bool linkProgram(unsigned int id);
};
