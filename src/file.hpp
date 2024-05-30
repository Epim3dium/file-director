#pragma once
#include <iostream>
#include "font_renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "glm/glm.hpp"
#include "mesh.h"
#include "shader.h"

#include <filesystem>

struct File  {
    static const unsigned int NUMBER_OF_VERTICES = 7;
    static const unsigned int VERTEX_SIZE = 3;
    const glm::vec3 FILE_COLOR = {1, 215.f / 255.f, 119.f / 255.f};
    const std::vector<Vertex> VERTICES = {
        {glm::vec3(0.0f,  0.0f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2( 0.0f, 0.0f)},  //center
        {glm::vec3(-1.0f, 1.0f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(-1.0f, 1.0f)},  //top-left corner
        {glm::vec3(-0.4f, 1.0f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(-0.4f, 1.0f)},  //end of the sticking out stub
        {glm::vec3(-0.2f, 0.8f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(-0.2f, 0.8f)},  //downslope of sticking out stub
        {glm::vec3(1.0f,  0.8f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2( 1.0f, 0.8f)},  //top-right corner
        {glm::vec3(1.0f,  -1.0f, 0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2( 1.0f,-1.0f)}, //bottom-right
        {glm::vec3(-1.0f, -1.0f, 0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(-1.0f,-1.0f)}, //bottom-left
        
        {glm::vec3(0.0f,  0.0f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2( 0.0f, 0.0f)},  //center
        {glm::vec3(-1.0f, 1.0f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(-1.0f, 1.0f)},  //top-left corner
        {glm::vec3(-0.4f, 1.0f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(-0.4f, 1.0f)},  //end of the sticking out stub
        {glm::vec3(-0.2f, 0.8f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(-0.2f, 0.8f)},  //downslope of sticking out stub
        {glm::vec3(1.0f,  0.8f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2( 1.0f, 0.8f)},  //top-right corner
        {glm::vec3(1.0f,  -1.0f,-0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2( 1.0f,-1.0f)}, //bottom-right
        {glm::vec3(-1.0f, -1.0f,-0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(-1.0f,-1.0f)}, //bottom-left
    };
    const std::vector<Vertex> TEXT_VERTICES = {
        {glm::vec3(-1.1, 1.1, 0), {}, {1, 0, 0}},
        {glm::vec3(1.1, 1.1, 0) , {}, {1, 0, 0}},
        {glm::vec3(1.1, 1.5, 0) , {}, {1, 0, 0}},
        {glm::vec3(-1.1, 1.5, 0), {}, {1, 0, 0}},
    };
    static const unsigned int TRIANGLE_COUNT = 6;
    static const unsigned int NUMBER_OF_INDICES = 3 * TRIANGLE_COUNT;
    const std::vector<GLuint> INDICES = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 1,
            
            7, 8, 9,
            7, 9, 10,
            7, 10, 11,
            7, 11, 12,
            7, 12, 13,
            7, 13, 8,
    };
    const std::vector<GLuint> TEXT_INDICES = {
        0, 1, 2,
        2, 3, 0
    };
    Mesh main_mesh;
    Mesh text_mesh;
    std::filesystem::path myPath;
    
    unsigned int vertexBufferId;
    unsigned int vertexArrayId;
    unsigned int indexBufferId;
    File(FontRenderer &fr,
         std::filesystem::path path = std::filesystem::current_path())
        : text_mesh(TEXT_VERTICES, TEXT_INDICES, {fr.generate(path.string(), {0, 1, 1}, 1)}), main_mesh(VERTICES, INDICES, {}), myPath(path) {}
    void setPath(const std::filesystem::path& p = std::filesystem::current_path()) {
        myPath = p;
    }
    void draw(Shader& shader, Camera& camera) {
        main_mesh.draw(shader, camera);
        text_mesh.draw(shader, camera);
    }
};
