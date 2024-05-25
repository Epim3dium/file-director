#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "glm/glm.hpp"
#include "shader.h"

struct Model {
    virtual void draw() {}
};
struct Folder : public Model {
    static const unsigned int NUMBER_OF_VERTICES = 7;
    static const unsigned int VERTEX_SIZE = 3;
    const float VERTICES[NUMBER_OF_VERTICES * VERTEX_SIZE] = {
            0.0f,  0.0f,  0.f,
            -0.5f, 0.5f,  0.f,
            -0.2f, 0.5f,  0.f,
            -0.1f, 0.4f,  0.f,
            0.5f,  0.4f,  0.f,
            0.5f,  -0.5f, 0.f,
            -0.5f, -0.5f, 0.f,
    };
    static const unsigned int TRIANGLE_COUNT = 6;
    static const unsigned int NUMBER_OF_INDICES = 3 * TRIANGLE_COUNT;
    const unsigned int INDICES[NUMBER_OF_INDICES] = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 1
    };
    unsigned int vertexBufferId;
    unsigned int vertexArrayId;
    unsigned int indexBufferId;
    Folder() {
    }
    void init() {
        glGenBuffers(1, &vertexBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vertexArrayId);
        glBindVertexArray(vertexArrayId);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, VERTEX_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), nullptr);

        glGenBuffers(1, &indexBufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
    }
    void draw() override final {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glBindVertexArray(vertexArrayId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        glDrawElements(GL_TRIANGLES, NUMBER_OF_INDICES, GL_UNSIGNED_INT, nullptr);
    }
};
