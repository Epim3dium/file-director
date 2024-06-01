#include <iostream>
#include <filesystem>
#include <thread>
#include "app.hpp"
#include "defs.h"
#include "shader.h"
#include "camera.h"
#include "skybox.h"
#include "file.hpp"
#include "input_parser.hpp"

#include "stb_image_write.h"
#include "stb_truetype.h"
#include "font_renderer.h"


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

#define PLANE_SIZE 1000.f
class Demo : public App {
public:
    float time = 0; 
    
    Skybox skybox;
    Shader spDefault;
    Shader spGrass;
    Shader spLambert;
    Shader spFile;
    Shader spText;
    
    FontRenderer font_renderer;
    File file;
    glm::vec2 noise_offset;
    Texture perlin;
    Texture grass_texture;
    Texture grass_detail;
    Mesh grass_plane;
    
    Camera camera;
    glm::mat4 Mk;
#define SUN_POSITION (glm::normalize(glm::vec3(3, 4, -10.2)) * 100000.f)

    float angle = 0.f;
    bool setup() override final {
        file.setPath();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        for(auto& tex : {perlin, grass_texture, grass_detail}) {
            tex.bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            tex.unbind();
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        spLambert.setUniform3f("lightPos", SUN_POSITION);
        spLambert.setUniform1f("ambientLight", 0.5f);
        return true;
    }
    void update() override final  {
        time += 1.0/60.0;
        
        camera.processInput(window);
        camera.update(0.1f, 100.0f, glm::radians(45.f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		Mk = glm::rotate(Mk, glm::radians(0.3f), glm::vec3(0.0f, 1.0f, 0.0f));
        spLambert.bind();
        // glUniformMatrix4fv(spLambert.u("M"), 1, false, glm::value_ptr(Mk));
        spText.bind();
        // glUniformMatrix4fv(spText.u("M"), 1, false, glm::value_ptr(Mk));
        file.draw(spLambert, spText, camera);
        
        const int maxLayer = 100;
        spGrass.setUniformMatrix4fv("M", glm::mat4(1));
        noise_offset.x =  time / 10.f;
        noise_offset.y =  time / 10.f;
        spGrass.setUniform2f("noiseOffset", noise_offset);
        spGrass.setUniform1f("maxGrassLength", 0.1f);
        spGrass.setUniform1f("maxLayer", maxLayer);
        
        grass_plane.draw(spGrass, camera, maxLayer);
        
        skybox.draw(camera);
    }
    void cleanup() override final {}
    Demo(int w, int h)
        : 
            font_renderer( FD_ASSET_DIR"/monospace.ttf", 1024, 32, 32),
            file(glm::vec3(0, 1, 3), glm::vec3(0.5, 0.5, 0.5), glm::mat4(1), font_renderer),
            grass_texture(FD_TEXTURE_DIR"/grass.jpg", "grass", 2),
            // grass_texture(font_renderer.generate("    XD", glm::vec3(1, 0, 0), 2)),
            grass_detail(FD_TEXTURE_DIR"/grass_detail.png", "detail", 0),
            perlin(FD_TEXTURE_DIR"/perlin_single.png", "noise", 1),
            skybox(FD_TEXTURE_DIR"/skybox"),
            
            spDefault(FD_SHADER_DIR"/v_default.glsl", FD_SHADER_DIR"/f_default.glsl"),
            spText(FD_SHADER_DIR"/v_default_text.glsl", FD_SHADER_DIR"/f_default_text.glsl"),
            spLambert(FD_SHADER_DIR"/v_lambert.glsl", FD_SHADER_DIR"/f_lambert.glsl"),
            spGrass(FD_SHADER_DIR"/v_grass.glsl", FD_SHADER_DIR"/f_grass.glsl"),
            spFile(spLambert),
            
            grass_plane({
                Vertex({-PLANE_SIZE, 0, -PLANE_SIZE}, {0, 1, 0}, {0, 0.5,  0}, {-PLANE_SIZE, -PLANE_SIZE}), 
                Vertex({-PLANE_SIZE, 0, PLANE_SIZE},  {0, 1, 0}, {0, 0.75, 0}, {-PLANE_SIZE, PLANE_SIZE}),
                Vertex({PLANE_SIZE,  0, PLANE_SIZE},  {0, 1, 0}, {0, 0.6,  0}, {PLANE_SIZE ,  PLANE_SIZE}),
                Vertex({PLANE_SIZE,  0, -PLANE_SIZE}, {0, 1, 0}, {0, 0.9,  0}, {PLANE_SIZE ,  -PLANE_SIZE})}, {0, 1, 2, 2, 3, 0}, {grass_texture, grass_detail, perlin}),
            
            camera(WIDTH, HEIGHT, glm::vec3(0, 1, -5), glm::vec3(0, 1, 1)), App(w, h) 
    {}
};

int main(int argc, char** argv) {
    InputParser input(argc, argv);
    Demo demo(WIDTH, HEIGHT);
    demo.run();

}
