#include <string>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include "shader.h"


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
GLuint Shader::u(const char* variableName) {
	return glGetUniformLocation(id,variableName);
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
    bool compiled = status != 0;
    if (!compiled) {
        fprintf(stderr, "Could not compile shader\n");
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
