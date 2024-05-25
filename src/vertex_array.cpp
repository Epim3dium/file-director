#include "vertex_array.h"
void VertexArray::link(VertexBuffer& buffer, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    buffer.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	buffer.unbind();
}
void VertexArray::bind() {
    glBindVertexArray(ID);
}
void VertexArray::unbind() {
    glBindVertexArray(0);
}
void VertexArray::remove() {
    glDeleteVertexArrays(1, &ID);
}

VertexArray::VertexArray() {
    glGenVertexArrays(1, &ID);
}
