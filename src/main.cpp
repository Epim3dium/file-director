#include <iostream>
#include <filesystem>
#include <thread>
#include "app.hpp"
#include "defs.h"
#include "shader.h"
#include "camera.h"
#include "skybox.h"
#include "folder.hpp"
#include "input_parser.hpp"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

#define PLANE_SIZE 100.f
class Demo : public App {
public:
    float time = 0; 
    
    Folder folder;
    Skybox skybox;
    Shader spDefault;
    Shader spGrass;
    
    glm::vec2 noise_offset;
    Texture perlin;
    Texture grass_texture;
    Texture grass_detail;
    Mesh grass_plane;
    
    Camera camera;

    float angle = 0.f;
    bool setup() override final {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        for(auto& tex : {perlin, grass_texture, grass_detail}) {
            tex.bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            tex.unbind();
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        return true;
        
        spDefault.bind();
        glUniform4f(spDefault.u("color"), 0, 1, 0, 1);
    }
    void update() override final  {
        time += 1.0/60.0;
        
        camera.processInput(window);
        camera.update(0.1f, 100.0f, glm::radians(45.f));
        angle += 0.1f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		glm::mat4 Mk = glm::rotate(glm::translate(vec3{0, 0.5, 0}), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        
        spDefault.bind();
        glUniformMatrix4fv(spDefault.u("M"), 1, false, glm::value_ptr(Mk));
        folder.draw(spDefault, camera);
        
        spGrass.bind();
        Mk = mat4(1);

        const int maxLayer = 200;
        glUniformMatrix4fv(spGrass.u("M"), 1, false, glm::value_ptr(Mk));
        
        noise_offset.x = cosf(time / M_PI / 5.f);
        noise_offset.y = sinf(time / M_PI / 5.f);
        glUniform2f(spGrass.u("noiseOffset"), noise_offset.x, noise_offset.y);
        glUniform1f(spGrass.u("maxGrassLength"), 0.1f);
        glUniform1f(spGrass.u("maxLayer"), maxLayer);
        
        grass_plane.draw(spGrass, camera, maxLayer);
        
        skybox.draw(camera);
    }
    void cleanup() override final {}
    Demo(int w, int h)
        : 
            grass_texture("../assets/grass.jpg", "grass", 0),
            grass_detail("../assets/grass_fur_copy.png", "detail", 1),
            perlin("../assets/perlin_single.png", "noise", 2),
            grass_plane({
                Vertex({-PLANE_SIZE, 0, -PLANE_SIZE}, {0, 1, 0}, {0, 0.5,  0}, {-PLANE_SIZE, -PLANE_SIZE}), 
                Vertex({-PLANE_SIZE, 0, PLANE_SIZE},  {0, 1, 0}, {0, 0.75, 0}, {-PLANE_SIZE, PLANE_SIZE}),
                Vertex({PLANE_SIZE,  0, PLANE_SIZE},  {0, 1, 0}, {0, 0.6,  0}, {PLANE_SIZE,  PLANE_SIZE}),
                Vertex({PLANE_SIZE,  0, -PLANE_SIZE}, {0, 1, 0}, {0, 0.9,  0}, {PLANE_SIZE,  -PLANE_SIZE})}, {0, 1, 2, 2, 3, 0}, {grass_texture, grass_detail, perlin}),
            spDefault("../assets/v_default.glsl", "../assets/f_default.glsl"),
            spGrass("../assets/v_grass.glsl", "../assets/f_grass.glsl"),
            skybox("../assets/skybox"),
            camera(WIDTH, HEIGHT, glm::vec3(0, 1, -5)), App(w, h) 
    {}
};

struct path_leaf_string
{
    std::string operator()(const std::filesystem::directory_entry& entry) const
    {
        return entry.path().string();
    }
};
 
std::vector<std::string> read_directory(const std::string& name)
{
    std::vector<std::string> v;
    std::filesystem::path p(name);
    std::filesystem::directory_iterator start(p);
    std::filesystem::directory_iterator end;
    std::transform(start, end, std::back_inserter(v), path_leaf_string());
    return v;
}

int main(int argc, char** argv) {
    InputParser input(argc, argv);
    std::cerr << std::filesystem::current_path() << ":\n";
    for(auto str : read_directory(std::filesystem::current_path().string())) {
        std::cerr << str << "\n";
    }
    
    return 0;
    if(argc == 2) {
        
    }
    Demo demo(WIDTH, HEIGHT);
    demo.run();

}
