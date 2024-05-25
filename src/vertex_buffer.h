#include "defs.h"
#include "vertex.hpp"
#include <vector>

struct VertexBuffer {
    GLuint ID;

    void bind();
    void unbind();
    void remove();
    
    VertexBuffer(const std::vector<Vertex>& verts);
};
