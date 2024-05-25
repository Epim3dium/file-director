#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "glm/glm.hpp"
#include "mesh.h"
#include "shader.h"

struct Model {
    virtual void draw() {}
};
struct Folder : public Model {
    static const unsigned int NUMBER_OF_VERTICES = 7;
    static const unsigned int VERTEX_SIZE = 3;
    const std::vector<Vertex> VERTICES = {
        {glm::vec3(0.0f,  0.0f,  0.f)},
        {glm::vec3(-0.5f, 0.5f,  0.f)},
        {glm::vec3(-0.2f, 0.5f,  0.f)},
        {glm::vec3(-0.1f, 0.4f,  0.f)},
        {glm::vec3(0.5f,  0.4f,  0.f)},
        {glm::vec3(0.5f,  -0.5f, 0.f)},
        {glm::vec3(-0.5f, -0.5f, 0.f)},
    };
    static const unsigned int TRIANGLE_COUNT = 6;
    static const unsigned int NUMBER_OF_INDICES = 3 * TRIANGLE_COUNT;
    const std::vector<GLuint> INDICES = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 1
    };
    Mesh mesh;
    unsigned int vertexBufferId;
    unsigned int vertexArrayId;
    unsigned int indexBufferId;
    Folder() : mesh(VERTICES, INDICES, {}) {
    }
    void draw(Shader& shader, Camera& camera) {
        mesh.draw(shader, camera);
    }
};
