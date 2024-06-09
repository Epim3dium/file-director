#pragma once
#include "mesh.h"
#include "shader.h"
struct Grass {
    glm::vec2 noise_offset;
    static constexpr int maxLayer = 50;
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::mat4 rotation = glm::mat4(1);
    Texture grass_texture;
    Texture grass_detail;
    Texture perlin;
    Mesh grass_plane;
    Grass(const char *grass_diffuse, const char *detail, const char *perlinfile, Mesh mesh)
        : grass_texture(grass_diffuse, "grass", 1),
          grass_detail(detail, "detail", 2), perlin(perlinfile, "noise", 3) ,
          grass_plane(mesh)
    {
        grass_plane.textures.clear();
        grass_plane.textures.push_back(grass_texture);
        grass_plane.textures.push_back(grass_detail);
        grass_plane.textures.push_back(perlin);
        for (auto &tex : {perlin, grass_texture, grass_detail}) {
            tex.bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            tex.unbind();
        }
    }
    void draw(Camera& camera, Shader& spGrass, float time) {
        glm::mat4 M = glm::mat4(1);
        
        M = glm::translate(M, position);
        M = M * rotation;
        M = glm::scale(M, scale);

        spGrass.setUniformMatrix4fv("M", M);
        noise_offset.x =  time / 10.f;
        noise_offset.y =  time / 10.f;
        spGrass.setUniform2f("noiseOffset", noise_offset);
        spGrass.setUniform1f("grass.length", 0.1f);
        spGrass.setUniform1f("grass.maxLayer", maxLayer);
        spGrass.setUniform1f("grass.maxDistance", 10.f);
        spGrass.setUniform1f("grass.padding", 0.1f);
        grass_plane.draw(spGrass, camera, maxLayer);
    }
};
