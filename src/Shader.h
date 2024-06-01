#pragma once

#include <unordered_map>
#include "defs.h"

class Shader {
private:
    unsigned int id = -1;

    void init(const std::string &vertexFile, const std::string &fragmentFile);
public:
    Shader(const std::string &vertexFile, const std::string &fragmentFile);
    ~Shader();

    void bind() const;
    void unbind() const;
    
    GLuint u(const char* variableName);
    void setUniform1f(const char* var, float value);
    void setUniform2f(const char* var, glm::vec2 value);
    void setUniform3f(const char* var, glm::vec3 value);
    void setUniform4f(const char* var, glm::vec4 value);
    void setUniformMatrix3fv(const char* var, glm::mat3 value);
    void setUniformMatrix4fv(const char* var, glm::mat4 value);
    GLuint a(const char* variableName);

private:
    unsigned int createShaderSource(unsigned int shaderType, const std::string &shaderSource);
    static void setShaderSource(unsigned int shaderId, const std::string &shaderSource);
    static bool compileShader(unsigned int shaderId);
    static unsigned int createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);
    static bool linkProgram(unsigned int id);
};
