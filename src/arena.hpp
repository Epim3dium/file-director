#pragma once
#include "file.h"
#include "grass.hpp"
#include <filesystem>
namespace fs = std::filesystem;
struct Arena {
    static inline int getDirCountAll(fs::path path) {
        auto dirIter = std::filesystem::directory_iterator(path);
        int fileCount = 0;
        for(const auto& entry : dirIter) {
            fileCount++;
        }
        return fileCount;
    }
    float arena_sides;
    float arena_size;

    Grass grass_cutout;

    Mesh gun;
    Mesh marble_plane;
    Mesh& column;


    float time = 0;

    std::vector<File> files;
        
    Arena(fs::path path, std::vector<Texture>& column_textures, Mesh& col, std::vector<Texture>& marble_textures, FontRenderer& font_renderer) :
          gun(FD_MODEL_DIR "/raygun.obj",
              {Texture(FD_TEXTURE_DIR "/raygun_diffuse.jpg", "diffuse", 1)}),
          column(col),
          arena_size(4.f * sqrt(getDirCountAll(path))),
          arena_sides(getDirCountAll(path)),
          grass_cutout(FD_TEXTURE_DIR "/grass.jpg", FD_TEXTURE_DIR "/grass_detail.png", FD_TEXTURE_DIR "/perlin_single.png", Mesh::UniformConvexCutout(200.f, arena_size, arena_sides)), 
          marble_plane(Mesh::UniformConvex(arena_size, arena_sides, marble_textures))
    {
        int i = 0;
        for(const auto& entry : fs::directory_iterator(path)) {
            float angle_step = 2.f * M_PI / arena_sides;
            float angle = angle_step / 2.f + angle_step * i++;
            glm::vec3 dir = {cos(angle), 0, sin(angle)};
            glm::mat4 R = glm::rotate(angle, vec3{0, 1, 0});
            files.push_back(File(dir * arena_size + vec3(0, 1, 0), vec3{1, 1, 1} * 0.5f, R, font_renderer, entry.path()));
        }
    }
    struct ShaderSet {
        Shader& spLambert;
        Shader& spText;
        Shader& spBlinnPhong;
        Shader& spGrass;
        Shader& spDefault;
    };
    bool justClickedBackspace = false;
    bool lastClickedBackspace = false;
    void draw(GLFWwindow* window, Camera& camera, ShaderSet shaders) {
        time += 1.0/60.0;

        float dist = INFINITY;
        fs::path closest;
        for(auto& f : files) {
            auto pos = findClosestPointOnRay(camera.position, camera.direction, f.position);
            auto len = glm::length(pos - f.position);
            if(len < dist && len < 2.f) {
                dist = len;
                closest = f.my_path;
            }
        }
        if(glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
            if(lastClickedBackspace == false) {
                justClickedBackspace = true;
                lastClickedBackspace = true;
            }else if(lastClickedBackspace == true){
                justClickedBackspace = false;
            }
        }else {
            lastClickedBackspace = false;
            justClickedBackspace = false;
        }

        for(auto& f : files) {
            glm::vec4 aura_color(0, 0, 0, 0);
            if(f.my_path == closest) {
                aura_color = glm::vec4(1, 1, 1, 1);
                if(justClickedBackspace) {
                    if(f.awaiting_deletion) {
                        f.isActive = false;
                    }
                    f.awaiting_deletion = true;
                }
            }else {
                f.awaiting_deletion = false;
            }
            f.draw(shaders.spLambert, shaders.spDefault, shaders.spText, camera, aura_color);
            f.rotation = glm::rotate(f.rotation, 0.01f, vec3(0, 1, 0));
        }

        shaders.spBlinnPhong.setUniform3f("light.ambient", glm::vec3(0.5, 0.5, 0.5));
        shaders.spBlinnPhong.setUniformMatrix4fv("M", glm::translate(glm::vec3{0, 0, 0}));
        marble_plane.draw(shaders.spBlinnPhong, camera);
        shaders.spBlinnPhong.setUniform3f("light.ambient", glm::vec3(0.2, 0.2, 0.2));

        for(float i = 0; i < arena_sides; i++) {
            float angle = 2.f * M_PI / arena_sides * i;
            glm::vec3 dir = {cos(angle), 0, sin(angle)};
            glm::mat4 M(1.f);
            M = glm::translate(dir * arena_size);
            M = glm::rotate(M, (float)M_PI * 0.5f - angle, {0, 1, 0});
            M = glm::scale(M, vec3(0.03, 0.03, 0.03));
            shaders.spBlinnPhong.setUniformMatrix4fv("M", M);
            column.draw(shaders.spBlinnPhong, camera);
        }

        grass_cutout.draw(camera, shaders.spGrass, time);

        
    }
};
