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
    static std::string get_homedir(void);
    static std::pair<Texture, float> genTextTexture(FontRenderer& fr, std::string str, glm::vec3 color) ;
    static std::vector<Vertex> genTextVerticies(float text_ratio, float text_height = 0.3) ;

    static const unsigned int NUMBER_OF_VERTICES = 7;
    static const unsigned int VERTEX_SIZE = 3;
    static const glm::vec3 FILE_COLOR;
    static const std::vector<Vertex> VERTICES;
    static const unsigned int TRIANGLE_COUNT = 6;
    static const unsigned int NUMBER_OF_INDICES = 3 * TRIANGLE_COUNT;
    static const std::vector<GLuint> INDICES;
    static const std::vector<GLuint> TEXT_INDICES;
    
    Mesh main_mesh;
    Mesh text_mesh;
    glm::vec3 position;
    glm::vec3 scale;
    glm::mat4 rotation = glm::mat4(1);
    
    std::filesystem::path path;
    
    File(glm::vec3 pos, glm::vec3 scale, glm::mat4 rot, FontRenderer &fr,
         std::filesystem::path path_ = std::filesystem::current_path());
    void draw(Shader& folder_shader, Shader& text_shader, Camera& camera);
};
