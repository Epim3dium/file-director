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
    void initVAO();

    void draw(Shader& shader, Camera& cam, GLsizei count = 1);
    void addTexture(const Texture& tex);
    void removeTexture(unsigned int ID);
    void generateTangents();
    static Mesh Plane(float size = 1, std::vector<Texture> textures = {});
    static Mesh UniformConvex(float radius, size_t sides, std::vector<Texture> textures = {});
    static Mesh UniformConvexCutout(float large_size, float radius, size_t sides, std::vector<Texture> textures = {});

    Mesh(std::string obj_filepath, const std::vector<Texture>& textures); 
    Mesh(const std::vector<Vertex>& verticies, const std::vector<GLuint>& indices, const std::vector<Texture>& textures); 
};
