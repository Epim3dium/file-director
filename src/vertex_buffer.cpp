#include "vertex_buffer.h"
void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VertexBuffer::remove() {
    glDeleteBuffers(1, &ID);
}

VertexBuffer::VertexBuffer(const std::vector<Vertex>& verts) {
    glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
}
