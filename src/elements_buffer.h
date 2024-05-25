#pragma once
#include "defs.h"
#include <vector>

struct ElementsBuffer {
    GLuint ID;

    ElementsBuffer(const std::vector<GLuint>& indices);
    void bind();
    void unbind();
    void remove();
};
