#pragma once
#include "file.h"
#include "grass.hpp"
#include "skybox.h"
#include <filesystem>
#include <stack>
namespace fs = std::filesystem;
struct Arena {
    static inline int getDirCountAll(fs::path path) {
        auto dirIter = std::filesystem::directory_iterator(path);
        int fileCount = 0;
        for(const auto& entry : dirIter) {
            fileCount++;
        }
        return std::max(fileCount, 4);
    }
    float arena_sides;
    float arena_size;

    Grass grass_cutout;

    Mesh gun;
    Mesh marble_plane;
    Mesh* column;
    FontRenderer* font_rend;
    Skybox* skybox;
    Skybox* inFileSkybox = nullptr;

    std::stack<fs::path> traversed;

    float time = 0;

    std::vector<File> files;
    fs::path from;
        
    Arena(fs::path path_, Mesh& col, std::vector<Texture>& marble_textures, FontRenderer& font_renderer, Skybox& skyb, std::stack<fs::path> history = {}) :
        traversed(history),
        from(path_),
        gun(FD_MODEL_DIR "/raygun.obj",
                {Texture(FD_TEXTURE_DIR "/raygun_diffuse.jpg", "diffuse", 1)}),
        column(&col),
        font_rend(&font_renderer),
        skybox(&skyb),
        arena_size(4.f * sqrt(getDirCountAll(path_))),
        arena_sides(getDirCountAll(path_)),
        grass_cutout(FD_TEXTURE_DIR "/grass.jpg", FD_TEXTURE_DIR "/grass_detail.png", FD_TEXTURE_DIR "/perlin_single.png", Mesh::UniformConvexCutout(200.f, arena_size, arena_sides)), 
        marble_plane(Mesh::UniformConvex(arena_size, arena_sides, marble_textures))
    {
        int i = 0;
        for(const auto& entry : fs::directory_iterator(path_)) {
            float angle_step = 2.f * M_PI / arena_sides;
            float angle = angle_step / 2.f + angle_step * i++;
            glm::vec3 dir = {cos(angle), 0, sin(angle)};
            glm::mat4 R = glm::rotate(angle, vec3{0, 1, 0});
            files.push_back(File(dir * arena_size + vec3(0, 1, 0), vec3{1, 1, 1} * 0.5f, R, font_renderer, entry.path()));
        }
        std::cerr << "Arena initialized\n";
    }
    struct ShaderSet {
        Shader& spLambert;
        Shader& spText;
        Shader& spBlinnPhong;
        Shader& spGrass;
        Shader& spDefault;
    };
    bool isInFile = false;
    bool justClickedMinus = false;
    bool lastClickedMinus = false;
    bool justClickedBackspace = false;
    bool lastClickedBackspace = false;
    bool lastClickedEnter = false;
    void draw(GLFWwindow* window, Camera& camera, ShaderSet shaders) {
        if(isInFile) {
            inFileSkybox->draw(camera);
            if(glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
                inFileSkybox->decomission();
                delete inFileSkybox;
                isInFile = false;
                camera.position = glm::vec3(0, 1, 0);
            }
            return;
        }
        float dist = INFINITY;
        fs::path closest_path;
        File* closest_file;
        for(auto& f : files) {
            auto pos = findClosestPointOnRay(camera.position, camera.direction, f.position);
            auto len = glm::length(pos - f.position);
            if(len < dist && len < 2.f) {
                dist = len;
                closest_path = f.my_path;
                closest_file = &f;
            }
        }
        
        if(justClickedMinus ) {
            if(!traversed.empty()) {
                auto target = traversed.top();
                traversed.pop();
                for(auto& f : files) {
                    f.decomission();
                }
                *this = Arena(target, *column, marble_plane.textures, *font_rend, *skybox, traversed);
                return;
            }else {
                std::cerr << "Can't go any higher in file structure\n";
            }
        }
        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            
            if(!lastClickedEnter) {
                //Entering
                if(closest_file->type == FileType::Dir ) {
                    for(auto& f : files) {
                        f.decomission();
                    }
                    traversed.push(from);
                    *this = Arena(closest_path, *column, marble_plane.textures, *font_rend, *skybox, traversed);
                    camera.position = {0, 1, 0};
                    return;
                }else if(closest_file->type == FileType::Readable) {
        
                    auto contents = dumpStringFromFile(closest_path.string());
                    contents = contents.substr(0, 1024);
        
                    std::array<Bitmap, 6> bitmaps;
                    const int width = 512;
                    const int height = 512;
                    for(int i = 0; i < 4; i++) {
                        bitmaps[i].data = new unsigned char [width * height] {0};
                        bitmaps[i].width = width;
                        bitmaps[i].height = height;
                        bitmaps[i].channelNumber = 1;
                    }
                    bitmaps[4] = font_rend->generateBitmap(contents, width, height, 16);
                    bitmaps[5] = font_rend->generateBitmap(contents, width, height, 16);
                    isInFile = true;
        
                    inFileSkybox = new Skybox(bitmaps);
                }else if(closest_file->type == FileType::Image) {
                    std::array<Bitmap, 6> bitmaps;
                    for(int i = 0; i < 6; i++) {
                        bitmaps[i] = Texture::loadFromFile(closest_path.string().c_str());
                    }
                    isInFile = true;
        
                    inFileSkybox = new Skybox(bitmaps);
                }
            }
        
            lastClickedEnter = true;
        }else {
            lastClickedEnter = false;
        }
        if(glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
            if(lastClickedBackspace == false) {
                justClickedBackspace = true;
                lastClickedBackspace = true;
            }else {
                justClickedBackspace = false;
            }
        }else {
            lastClickedBackspace = false;
            justClickedBackspace = false;
        }
        if(glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            if(lastClickedMinus == false) {
                justClickedMinus = true;
                lastClickedMinus = true;
            }else {
                justClickedMinus = false;
            }
        }else {
            lastClickedMinus = false;
            justClickedMinus = false;
        }

        time += 1.0/60.0;


        for(auto& f : files) {
            if(f.my_path == closest_path) {
                if(justClickedBackspace) {
                    if(f.awaiting_deletion) {
                        f.isActive = false;
                    }
                    f.awaiting_deletion = true;
                }

                f.selected = true;
            }else {
                f.selected = false;
                f.awaiting_deletion = false;
            }
            f.draw(shaders.spLambert, shaders.spDefault, shaders.spText, camera);
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
            column->draw(shaders.spBlinnPhong, camera);
        }

        grass_cutout.draw(camera, shaders.spGrass, time);

        
        skybox->draw(camera);
    }
};
