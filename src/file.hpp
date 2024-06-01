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
    static inline std::string get_homedir(void) {
        std::string homedir;
#ifdef _WIN32
        homedir = getenv("HOMEDRIVE");
        homedir += getenv("HOMEPATH");
#else
        homedir = getenv("HOME");
#endif
        return homedir;
    }
    static inline std::pair<Texture, float> genTextTexture(FontRenderer& fr, std::string str, glm::vec3 color) {
        constexpr float SCALE = 2.f;
        constexpr float FONT_HEIGHT = 32.f * SCALE;
        constexpr float FONT_WIDTH = 20.f * SCALE;

        // int width = fr.getPixelWidth(str, FONT_HEIGHT) + 4 + 71;
        int width = FONT_WIDTH * str.size();
        double font_ratio = width / FONT_HEIGHT ;
        return {fr.generate(str, color, width, FONT_HEIGHT , 1), font_ratio};
    }
    static inline std::vector<Vertex> genTextVerticies(float text_ratio, float text_height = 0.3) {
        const std::vector<Vertex> TEXT_VERTICES = {
            {glm::vec3(-text_height*text_ratio/2, 1.1, 0), {}, {1, 0, 0}, glm::vec2(0, 1)},
            {glm::vec3( text_height*text_ratio/2, 1.1, 0) , {}, {1, 0, 0}, glm::vec2(1, 1)},
            {glm::vec3( text_height*text_ratio/2, 1.1 + text_height, 0) , {}, {1, 0, 0}, glm::vec2(1, 0)},
            {glm::vec3(-text_height*text_ratio/2, 1.1 + text_height, 0), {}, {1, 0, 0}, glm::vec2(0, 0)},
        };
        return TEXT_VERTICES;
    }
    static std::filesystem::path startingAppPath() {
        static std::filesystem::path starting_path = std::filesystem::current_path();
        return starting_path; 
    }
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
    glm::vec3 position;
    glm::vec3 scale;
    glm::mat4 rotation = glm::mat4(1);
    
    std::string myPath;
    
    File(glm::vec3 pos, glm::vec3 scale, glm::mat4 rot, FontRenderer &fr,
         std::filesystem::path path = std::filesystem::current_path())
        // : text_mesh({}, {}, {}), main_mesh(VERTICES, INDICES, {}), myPath(path) 
        : position(pos), scale(scale), rotation(rot), text_mesh({}, {}, {}), main_mesh(VERTICES, INDICES, {}), myPath(path) 
    {
        auto home = get_homedir();
        auto curpath = std::filesystem::current_path().string();
        if(curpath.find(home) != std::string::npos) {
            curpath.erase(curpath.find(home), home.size());
        }
        auto [tex, ratio] = genTextTexture(fr, curpath, vec3(1, 1, 1));
        auto verts = genTextVerticies(ratio);
        text_mesh = Mesh(verts, TEXT_INDICES, {tex});
    }
    void setPath(const std::filesystem::path& p = std::filesystem::current_path()) {
        myPath = p;
    }
    void draw(Shader& folder_shader, Shader& text_shader, Camera& camera) {
        glm::mat4 Mf = glm::mat4(1);
        
        Mf = glm::translate(Mf, position);
        Mf = Mf * rotation;
        Mf = glm::scale(Mf, scale);
        
        auto Mt = Mf;
        Mt = glm::scale(Mt, glm::vec3(1,-1,1));
        
        auto proj = glm::vec3(camera.position.x, position.y, camera.position.z);
        Mt = Mt * glm::lookAt(glm::vec3(0, 0, 0), proj - position, -vec3(0, 1, 0));
        
        folder_shader.setUniformMatrix4fv("M", Mf);
        main_mesh.draw(folder_shader, camera);

        text_shader.setUniformMatrix4fv("M", Mt);
        text_mesh.draw(text_shader, camera);
    }
};
