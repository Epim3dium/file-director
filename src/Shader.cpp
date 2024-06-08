#include <iostream>
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include "shader.h"
#include <vector>


void Shader::init(const std::string &vertexFile, const std::string &fragmentFile) {
    assert(this->id == -1);
    auto vertexSource = dumpStringFromFile(vertexFile);
    auto fragmentSource  = dumpStringFromFile(fragmentFile);
    
    unsigned int vertexShaderId = createShaderSource(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShaderId = createShaderSource(GL_FRAGMENT_SHADER, fragmentSource);
    if (vertexShaderId != 0 && fragmentShaderId != 0) {
        id = createProgram(vertexShaderId, fragmentShaderId);
    } else {
        fprintf(stderr, "Could not create shader program\n");
    }
}
Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile) {
    init(vertexFile, fragmentFile);
}
void Shader::setUniform1f(const char* var, float value) const {
    bind();
    glUniform1f(u(var), value);
    unbind();
}
void Shader::setUniform2f(const char* var, glm::vec2 value) const {
    bind();
    glUniform2f(u(var), value.x, value.y);
    unbind();
}
void Shader::setUniform3f(const char* var, glm::vec3 value) const {
    bind();
    glUniform3f(u(var), value.x, value.y, value.z);
    unbind();
}
void Shader::setUniform4f(const char* var, glm::vec4 value) const {
    bind();
    glUniform4f(u(var), value.x, value.y, value.z, value.w);
    unbind();
}
void Shader::setUniformMatrix3fv(const char* var, glm::mat3 value) const {
    bind();
    glUniformMatrix3fv(u(var), 1, 0, glm::value_ptr(value));
    unbind();
}
void Shader::setUniformMatrix4fv(const char* var, glm::mat4 value) const {
    bind();
    glUniformMatrix4fv(u(var), 1, 0, glm::value_ptr(value));
    unbind();
}
GLuint Shader::u(const char* variableName) const {
    bind();
	return glGetUniformLocation(id,variableName);
    unbind();
}

GLuint Shader::a(const char* variableName) {
	return glGetAttribLocation(id,variableName);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::bind() const {
    glUseProgram(id);
}

void Shader::unbind() const {
    glUseProgram(0);
}

unsigned int Shader::createShaderSource(unsigned int shaderType, const std::string &shaderSource) {
    unsigned int shaderId = glCreateShader(shaderType);
    setShaderSource(shaderId, shaderSource);
    bool compiled = compileShader(shaderId);
    if (compiled) {
        return shaderId;
    } else {
        glDeleteShader(shaderId);
        return 0;
    }
}

void Shader::setShaderSource(unsigned int shaderId, const std::string &shaderSource) {
    char *source = (char *) shaderSource.c_str();
    unsigned int count = 1;
    int *length = nullptr;
    glShaderSource(shaderId, count, &source, length);
}

bool Shader::compileShader(unsigned int shaderId) {
    glCompileShader(shaderId);
    int status = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    bool compiled = status != GL_FALSE;
    if (!compiled) {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
        std::cerr << "Could not compile shader: ";
        std::cerr << errorLog.data();

        glDeleteShader(shaderId); // Don't leak the shader.
    }
    return compiled;
}

unsigned int Shader::createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId) {
    unsigned int programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    bool linked = linkProgram(programId);
    if (linked) {
        glDetachShader(programId, vertexShaderId);
        glDetachShader(programId, fragmentShaderId);
        return programId;
    } else {
        glDeleteProgram(programId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        return 0;
    }
}

bool Shader::linkProgram(unsigned int programId) {
    glLinkProgram(programId);
    int status = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, (int *) &status);
    bool linked = status != 0;
    if (!linked) {
        fprintf(stderr, "Could not link shader program\n");
    }
    return linked;
}
