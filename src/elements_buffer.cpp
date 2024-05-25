#pragma once
#include "elements_buffer.h"
ElementsBuffer::ElementsBuffer(const std::vector<GLuint>& indices) {
    glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}
void ElementsBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
void ElementsBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void ElementsBuffer::remove() {
    glDeleteBuffers(1, &ID);
}
