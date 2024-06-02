#pragma once
#include <iostream>
#include <filesystem>
#include <thread>
#include "defs.h"
#include "shader.h"
#include "camera.h"
#include "skybox.h"
#include "glad/glad.h"

const unsigned int DEFAULT_WIDTH = 800;
const unsigned int DEFAULT_HEIGHT = 800;


class App {
    float currentFrame;
    float passedTime;
    float lastFrame;
    static bool isGLError() {
        while (GLenum error = glGetError()) {
            std::cout << "GL error [" << error << "]" << std::endl;
            return true;
        }
        return false;
    }
protected:
    float target_FPS = 60.f;
    bool isActive = true;
    GLFWwindow* window;
public:
    virtual bool setup() {return true;}
    virtual void update() {}
    virtual void cleanup() {}

    App(int width, int height) {
        std::cout << "Initializing GLFW" << std::endl;
        if (!glfwInit()) {
            fprintf(stderr, "Could not initialize GLFW\n");
            assert(false);
        }

        std::cout << "Setting GLFW window hints" << std::endl;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        std::cout << "Creating GLFW window" << std::endl;
        window = glfwCreateWindow(width, height, "Untitled", nullptr, nullptr);
        if (window == nullptr) {
            fprintf(stderr, "Could not create GLFW window\n");
            glfwTerminate();
            assert(false);
        }

        std::cout << "Configuring GLFW window" << std::endl;
        glfwMakeContextCurrent(window);
        // glfwSetKeyCallback(window, onKeyPressed);
        // glfwSetMouseButtonCallback(window, mouse_button_callback);

        std::cout << "Initializing GLAD" << std::endl;
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            fprintf(stderr, "Could not initialize GLAD\n");
            assert(false);
        }

        std::cout << "Graphics context:" << std::endl;
        std::cout << "-- Vendor " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "-- Renderer " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "-- Version " << glGetString(GL_VERSION) << std::endl;
        glEnable(GL_DEPTH_TEST);  
    }

    int run() {
        
        if(!setup()) {
            return -1;
        }
        while (isActive && !glfwWindowShouldClose(window)) {
            currentFrame = glfwGetTime();
            passedTime += currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            glfwPollEvents();
            
            if(passedTime < 1.0 / target_FPS) {
                continue;
            }
            passedTime = 0.f;

            
            
            update();
                
            if (isGLError()) {
                fprintf(stderr, "Could not render\n");
                return -1;
            }

            glfwSwapBuffers(window);
        }
        cleanup();
        std::cout << "Destroying window" << std::endl;
        glfwDestroyWindow(window);
        std::cout << "Terminating GLFW" << std::endl;
        glfwTerminate();
        return 0;
    }
    
};
