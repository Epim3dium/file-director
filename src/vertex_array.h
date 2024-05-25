#pragma once
#include "vertex_buffer.h"

struct VertexArray {
    GLuint ID;

    void link(VertexBuffer& buffer, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void bind();
    void unbind();
    void remove();

    VertexArray();
};
