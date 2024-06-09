#include <filesystem>
#include "app.hpp"
#include "arena.hpp"
#include "defs.h"
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
    Shader spLambert;
    Shader spFile;
    Shader spText;
    Shader spGrass;
    
    Texture default_texture;
    std::vector<Texture> column_textures;
    std::vector<Texture> marble_textures;

    Mesh column;
    
    FontRenderer font_renderer;

    
    Camera camera;
    Arena arena;
#define SUN_DIR (-glm::normalize(glm::vec3(3, 4, -10.2)))
#define SUN_POSITION (-SUN_DIR * 100000.f)

    static constexpr float arena_size = 10.f;
    static constexpr float arena_sides = 5.f;
    float angle = 0.f;
    bool setup() override final {
        std::cerr << "setupping\n";
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        for(auto& tex : {default_texture}) {
            tex.bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            tex.unbind();
        }
        for(auto& tex : marble_textures) {
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
                                                                                
        spLambert.setUniform4f("light.position", glm::vec4(SUN_POSITION, 1));   
        spLambert.setUniform3f("light.color",    glm::vec3(1,   1,   1));       
        spLambert.setUniform3f("light.specular", glm::vec3(1,   1,   1));       
        spLambert.setUniform3f("light.diffuse",  glm::vec3(0.5, 0.5, 0.5));     
        spLambert.setUniform3f("light.ambient",  glm::vec3(0.2, 0.2, 0.2));     

        spGrass.setUniform4f("light.position", glm::vec4(SUN_POSITION, 1));   
        spGrass.setUniform3f("light.color",    glm::vec3(1,   1,   1));       
        spGrass.setUniform3f("light.specular", glm::vec3(1,   1,   1));       
        spGrass.setUniform3f("light.diffuse",  glm::vec3(0.2, 0.2, 0.2));     
        spGrass.setUniform3f("light.ambient",  glm::vec3(0.5, 0.5, 0.5));     
        std::cerr << "EOS\n";
        return true;
    }
    void update() override final  {
        camera.processInput(window);
        camera.update(0.1f, 200.0f, glm::radians(45.f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        arena.draw(window, camera, {spLambert, spText, spBlinnPhong, spGrass, spDefault});
        
    }
    void cleanup() override final {}
    Demo(const Demo &) = default;
    Demo(Demo &&) = default;
    Demo(int w, int h)
        : font_renderer(FD_ASSET_DIR "/monospace.ttf"),
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

          default_texture(FD_TEXTURE_DIR"/default.png", "none", 0),
          column_textures(
              {Texture(FD_TEXTURE_DIR "/column/diffuse.jpg", "diffuse", 1),
               Texture(FD_TEXTURE_DIR "/column/specular.jpg", "specular", 2),
               Texture(FD_TEXTURE_DIR "/column/normal.jpg", "normal", 3),
               Texture(FD_TEXTURE_DIR "/column/height.jpg", "height", 4)}),
          marble_textures(
              {Texture(FD_TEXTURE_DIR "/marble/diffuse.jpeg", "diffuse", 1),
               Texture(FD_TEXTURE_DIR "/marble/specular.jpeg", "specular", 2),
               Texture(FD_TEXTURE_DIR "/marble/normal.jpeg", "normal", 4),
               Texture(FD_TEXTURE_DIR "/marble/height.jpeg", "height", 6)}),
          column(FD_MODEL_DIR "/column_01.obj", column_textures),

          camera(WIDTH, HEIGHT, glm::vec3(0, 1, -5), glm::vec3(0, 1, 1)),
          arena(fs::current_path(), column, marble_textures, font_renderer, skybox),
          App(w, h) {}
};

int main(int argc, char** argv) {
    InputParser input(argc, argv);
    Demo demo(WIDTH, HEIGHT);
    std::cerr << "running:\n";
    demo.run();

}
