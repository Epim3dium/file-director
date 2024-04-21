#pragma once

#include <unordered_map>

class Shader {
private:
    unsigned int id;

public:
    Shader(const std::string &vertexFile, const std::string &fragmentFile);
    ~Shader();

    void Bind() const;
    void Unbind() const;

private:
    unsigned int createShaderSource(unsigned int shaderType, const std::string &shaderSource);
    static void setShaderSource(unsigned int shaderId, const std::string &shaderSource);
    static bool compileShader(unsigned int shaderId);
    static unsigned int createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);
    static bool linkProgram(unsigned int id);
};
