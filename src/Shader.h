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
    
    GLuint u(const char* variableName) const;
    void setUniform1f(const char* var, float value)           const ;
    void setUniform2f(const char* var, glm::vec2 value)       const ;
    void setUniform3f(const char* var, glm::vec3 value)       const ;
    void setUniform4f(const char* var, glm::vec4 value)       const ;
    void setUniformMatrix3fv(const char* var, glm::mat3 value)const ;
    void setUniformMatrix4fv(const char* var, glm::mat4 value)const ;
    GLuint a(const char* variableName);

private:
    unsigned int createShaderSource(unsigned int shaderType, const std::string &shaderSource);
    static void setShaderSource(unsigned int shaderId, const std::string &shaderSource);
    static bool compileShader(unsigned int shaderId);
    static unsigned int createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);
    static bool linkProgram(unsigned int id);
};
