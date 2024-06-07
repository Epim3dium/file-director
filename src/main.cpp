#include <iostream>
#include <filesystem>
#include <thread>
#include "app.hpp"
#include "defs.h"
#include "file.h"
#include "shader.h"
#include "camera.h"
#include "skybox.h"
#include "input_parser.hpp"

#include "stb_image_write.h"
#include "stb_truetype.h"
#include "font_renderer.h"


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

#define PLANE_SIZE 10.f
class Demo : public App {
public:
    float time = 0; 
    
    Skybox skybox;
    Shader spDefault;
    Shader spBlinnPhong;
    Shader spGrass;
    Shader spLambert;
    Shader spFile;
    Shader spText;
    
    FontRenderer font_renderer;

    File file;

    glm::vec2 noise_offset;
    Texture perlin;

    Texture default_texture;
    Texture grass_texture;
    Texture grass_detail;
    std::vector<Texture> column_textures;
    std::vector<Texture> marble_textures;

    Mesh plane;
    Mesh grass_plane;
    Mesh gun;
    Mesh column;
    
    Camera camera;
    glm::mat4 Mk;
#define SUN_DIR (glm::normalize(glm::vec3(3, 4, -10.2)))
#define SUN_POSITION (SUN_DIR * 100000.f)

    float angle = 0.f;
    bool setup() override final {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        for(auto& tex : {perlin, grass_texture, grass_detail, default_texture}) {
            tex.bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            tex.unbind();
        }
        default_texture.bind();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        spBlinnPhong.setUniform4f("light.position", glm::vec4(SUN_POSITION, 1));
        spBlinnPhong.setUniform3f("light.color",    glm::vec3(1,   1,   1));
        spBlinnPhong.setUniform3f("light.specular", glm::vec3(1,   1,   1));
        spBlinnPhong.setUniform3f("light.diffuse",  glm::vec3(0.5, 0.5, 0.5));
        spBlinnPhong.setUniform3f("light.ambient",  glm::vec3(0.2, 0.2, 0.2));
        glm::vec4 fLight = glm::normalize(camera.V *glm::vec4(SUN_POSITION, 1) - camera.V * mat4(1) * glm::vec4(0, 0, 0, 1)); //vector towards the light in eye space
        spLambert.setUniform4f("light.position", glm::vec4(SUN_POSITION, 1));
        spLambert.setUniform3f("light.color",    glm::vec3(1,   1,   1));
        spLambert.setUniform3f("light.specular", glm::vec3(1,   1,   1));
        spLambert.setUniform3f("light.diffuse",  glm::vec3(0.5, 0.5, 0.5));
        spLambert.setUniform3f("light.ambient",  glm::vec3(0.2, 0.2, 0.2));
        return true;
    }
    void update() override final  {
        time += 1.0/60.0;
        
        camera.processInput(window);
        camera.update(0.1f, 200.0f, glm::radians(45.f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		Mk = glm::rotate(Mk, glm::radians(0.3f), glm::vec3(0.0f, 1.0f, 0.0f));
        file.draw(spLambert, spText, camera);
        spBlinnPhong.setUniformMatrix4fv("M", glm::scale(glm::translate(glm::vec3{0, 1, 0}), vec3(2, 2, 2)));
        spBlinnPhong.setUniform3f("light.ambient",  glm::vec3(0.5, 0.5, 0.5));
        plane.draw(spBlinnPhong, camera);
        spBlinnPhong.setUniform3f("light.ambient",  glm::vec3(0.2, 0.2, 0.2));

        spBlinnPhong.setUniformMatrix4fv("M", glm::scale(glm::translate(glm::vec3(0, 1, 10)), vec3(0.1, 0.1, 0.1)));
        column.draw(spBlinnPhong, camera);

        
        const int maxLayer = 100;
        spGrass.setUniformMatrix4fv("M", glm::mat4(1));
        noise_offset.x =  time / 10.f;
        noise_offset.y =  time / 10.f;
        spGrass.setUniform2f("noiseOffset", noise_offset);
        spGrass.setUniform1f("maxGrassLength", 0.1f);
        spGrass.setUniform1f("maxLayer", maxLayer);
        for(auto& tex : marble_textures) {
            plane.addTexture(tex);
        }
        
        grass_plane.draw(spGrass, camera, maxLayer);
        
        skybox.draw(camera);
    }
    void cleanup() override final {}
    Demo(const Demo &) = default;
    Demo(Demo &&) = default;
    Demo &operator=(const Demo &) = default;
    Demo &operator=(Demo &&) = default;
    Demo(int w, int h)
        : font_renderer(FD_ASSET_DIR "/monospace.ttf"),
          file(glm::vec3(0, 1, 5), glm::vec3(0.5, 0.5, 0.5), glm::mat4(1),
               font_renderer, std::filesystem::temp_directory_path()),
          grass_texture(FD_TEXTURE_DIR "/grass.jpg", "grass", 1),
          // grass_texture(font_renderer.generate("    XD", glm::vec3(1, 0, 0),
          // 2)),
          grass_detail(FD_TEXTURE_DIR "/grass_detail.png", "detail", 2),
          perlin(FD_TEXTURE_DIR "/perlin_single.png", "noise", 3),
          skybox(FD_TEXTURE_DIR "/skybox"),

          spBlinnPhong(FD_SHADER_DIR "/v_blinn_phong.glsl",
                       FD_SHADER_DIR "/f_blinn_phong.glsl"),
          spDefault(FD_SHADER_DIR "/v_default.glsl",
                    FD_SHADER_DIR "/f_default.glsl"),
          spText(FD_SHADER_DIR "/v_default_text.glsl",
                 FD_SHADER_DIR "/f_default_text.glsl"),
          spLambert(FD_SHADER_DIR "/v_lambert.glsl",
                    FD_SHADER_DIR "/f_lambert.glsl"),
          spGrass(FD_SHADER_DIR "/v_grass.glsl", FD_SHADER_DIR "/f_grass.glsl"),
          spFile(spLambert),

          gun(FD_MODEL_DIR "/raygun.obj",
              {Texture(FD_TEXTURE_DIR "/raygun_diffuse.jpg", "diffuse", 1)}),
          default_texture(FD_TEXTURE_DIR"/default.png", "none", 0),
          column_textures(
              {Texture(FD_TEXTURE_DIR "/column/diffuse.jpg", "diffuse", 1),
               Texture(FD_TEXTURE_DIR "/column/specular.jpg", "specular", 2),
               Texture(FD_TEXTURE_DIR "/column/glossiness.jpg", "gloss", 3)}),
          marble_textures(
              {Texture(FD_TEXTURE_DIR "/marble/diffuse.jpeg", "diffuse", 1),
               Texture(FD_TEXTURE_DIR "/marble/specular.jpeg", "specular", 2),
               Texture(FD_TEXTURE_DIR "/marble/glossiness.jpeg", "gloss", 3),
               Texture(FD_TEXTURE_DIR "/marble/normal.jpeg", "normal", 4),
               Texture(FD_TEXTURE_DIR "/marble/roughness.jpeg", "rough", 5),
               Texture(FD_TEXTURE_DIR "/marble/height.jpeg", "bump", 6)}),
          column(FD_MODEL_DIR "/column_01.obj", column_textures),
          plane(Mesh::Plane()),
          grass_plane({Vertex({-PLANE_SIZE, 0, -PLANE_SIZE}, {0, 1, 0},
                              {0, 0.5, 0}, {-PLANE_SIZE, -PLANE_SIZE}),
                       Vertex({-PLANE_SIZE, 0, PLANE_SIZE}, {0, 1, 0},
                              {0, 0.75, 0}, {-PLANE_SIZE, PLANE_SIZE}),
                       Vertex({PLANE_SIZE, 0, PLANE_SIZE}, {0, 1, 0},
                              {0, 0.6, 0}, {PLANE_SIZE, PLANE_SIZE}),
                       Vertex({PLANE_SIZE, 0, -PLANE_SIZE}, {0, 1, 0},
                              {0, 0.9, 0}, {PLANE_SIZE, -PLANE_SIZE})},
                      {0, 1, 2, 2, 3, 0},
                      {grass_texture, grass_detail, perlin}),

          camera(WIDTH, HEIGHT, glm::vec3(0, 1, -5), glm::vec3(0, 1, 1)),
          App(w, h) {}
};

int main(int argc, char** argv) {
    InputParser input(argc, argv);
    Demo demo(WIDTH, HEIGHT);
    demo.run();

}
