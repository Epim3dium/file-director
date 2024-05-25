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

class Demo : public App {
public:
    Folder folder;
    Skybox skybox;
    Shader spLambert;
    Camera cam;

    float angle = 0.f;
    bool setup() override final {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        return true;
    }
    void update() override final  {
        cam.processInput(window);
        cam.update(0.1f, 100.0f, glm::radians(45.f));
        angle += 0.1f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		glm::mat4 Mk = glm::rotate(glm::mat4(1.f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        spLambert.bind();
        glUniform4f(spLambert.u("color"), 0, 1, 0, 1);
        glUniformMatrix4fv(spLambert.u("M"), 1, false, glm::value_ptr(Mk));
        // cam.exportMatrix(spLambert, "cam_mat");
        folder.draw(spLambert, cam);
        skybox.draw(cam);
    }
    void cleanup() override final {}
    Demo(int w, int h)
        : spLambert("../assets/v_lambert.glsl", "../assets/f_lambert.glsl"),
          cam(WIDTH, HEIGHT, glm::vec3(0, 0, -5)), App(w, h),
          skybox("../assets/skybox") {}
};


int main() {
    Demo demo(WIDTH, HEIGHT);
    demo.run();

}
