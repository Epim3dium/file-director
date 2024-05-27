#pragma once
#include "defs.h"
#include "vertex.hpp"
#include "texture.h"
#include "camera.h"
#include "vertex_array.h"
#include <vector>

struct Mesh {
    std::vector<Vertex> verticies;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VertexArray VAO;

    void draw(Shader& shader, Camera& cam, GLsizei count = 1);

    Mesh(const std::vector<Vertex>& verticies, const std::vector<GLuint>& indices, const std::vector<Texture>& textures); 
};
