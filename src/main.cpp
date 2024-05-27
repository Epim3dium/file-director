#include <iostream>
#include <filesystem>
#include <thread>
#include "app.hpp"
#include "defs.h"
#include "shader.h"
#include "camera.h"
#include "skybox.h"
#include "folder.hpp"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

#define PLANE_SIZE 1.f
class Demo : public App {
public:
    Folder folder;
    Skybox skybox;
    Shader spDefault;
    Shader spGrass;
    
    Texture grass_texture;
    Mesh grass_plane;
    
    Camera camera;

    float angle = 0.f;
    bool setup() override final {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        return true;
        
        spDefault.bind();
        glUniform4f(spDefault.u("color"), 0, 1, 0, 1);
    }
    void update() override final  {
        camera.processInput(window);
        camera.update(0.1f, 100.0f, glm::radians(45.f));
        angle += 0.1f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		glm::mat4 Mk = glm::rotate(mat4(1), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        
        spDefault.bind();
        glUniformMatrix4fv(spDefault.u("M"), 1, false, glm::value_ptr(Mk));
        folder.draw(spDefault, camera);
        spGrass.bind();
        Mk = mat4(1);
        glUniformMatrix4fv(spGrass.u("M"), 1, false, glm::value_ptr(Mk));
        grass_plane.draw(spGrass, camera);
        
        skybox.draw(camera);
    }
    void cleanup() override final {}
    Demo(int w, int h)
        : 
            grass_texture("../assets/grass.jpg", "grass", 0),
            grass_plane({
                Vertex({-PLANE_SIZE, 0, -PLANE_SIZE}, {0, 1, 0}, {0, 0.5,  0}, {-1, -1}), 
                Vertex({-PLANE_SIZE, 0, PLANE_SIZE},  {0, 1, 0}, {0, 0.75, 0}, {-1, 1}),
                Vertex({PLANE_SIZE,  0, PLANE_SIZE},  {0, 1, 0}, {0, 0.6,  0}, {1,  1}),
                Vertex({PLANE_SIZE,  0, -PLANE_SIZE}, {0, 1, 0}, {0, 0.9,  0}, {1,  -1})}, {0, 1, 2, 2, 3, 0}, {grass_texture}),
            spDefault("../assets/v_default.glsl", "../assets/f_default.glsl"),
            spGrass("../assets/v_grass.glsl", "../assets/f_grass.glsl"),
            skybox("../assets/skybox"),
            camera(WIDTH, HEIGHT, glm::vec3(0, 1, -5)), App(w, h) 
    {}
};


int main() {
    Demo demo(WIDTH, HEIGHT);
    demo.run();

}
