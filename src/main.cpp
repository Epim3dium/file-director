#include <iostream>
#include <filesystem>
#include <thread>
#include "defs.h"
#include "shader.h"
#include "camera.h"
#include "skybox.h"
#include "folder.hpp"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;



Shader spLambert;
Camera cam(WIDTH, HEIGHT, glm::vec3(0, 0, -5));

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
}
void onKeyPressed(GLFWwindow *window, int key, int scanCode, int action, int mode) {
}

bool isGLError() {
    while (GLenum error = glGetError()) {
        std::cout << "GL error [" << error << "]" << std::endl;
        return true;
    }
    return false;
}

int main() {
    std::cout << "Initializing GLFW" << std::endl;
    if (!glfwInit()) {
        fprintf(stderr, "Could not initialize GLFW\n");
        return -1;
    }

    std::cout << "Setting GLFW window hints" << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    std::cout << "Creating GLFW window" << std::endl;
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Untitled", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Could not create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    std::cout << "Configuring GLFW window" << std::endl;
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, onKeyPressed);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    std::cout << "Initializing GLAD" << std::endl;
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Could not initialize GLAD\n");
        return -1;
    }

    std::cout << "Graphics context:" << std::endl;
    std::cout << "-- Vendor " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "-- Renderer " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "-- Version " << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);  


    // folder.init();
    spLambert.init("../assets/v_lambert.glsl", "../assets/f_lambert.glsl");
    
    Folder folder;
    Skybox skybox("../assets/skybox");
    
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    float angle = 0.f;
    
    
    float currentFrame;
    float passedTime;
    float lastFrame;
    const float target_FPS = 60.f;

    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        passedTime += currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents();
        
        if(passedTime < 1.0 / target_FPS) {
            continue;
        }
        passedTime = 0.f;

        
        
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
        

        if (isGLError()) {
            fprintf(stderr, "Could not render\n");
            return -1;
        }

        glfwSwapBuffers(window);
    }

    std::cout << "Destroying window" << std::endl;
    glfwDestroyWindow(window);
    std::cout << "Terminating GLFW" << std::endl;
    glfwTerminate();
    return 0;
}
