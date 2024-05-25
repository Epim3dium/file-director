#pragma once
#include "shader.h"
#include "defs.h"

using glm::vec3;
using glm::mat4;
struct Camera {
    vec3 position;
    vec3 direction = vec3(0, 0, 1);
    
    float move_speed = 0.1f;
    float turn_speed = 0.2f;
    
    vec3 up = vec3(0, 1, 0);
    mat4 mat = mat4(1);

    struct {
        int width;
        int height;
    }window_size;

    bool firstClick;
    glm::vec2 lastMousePos; 

    void update(float nearClip, float farClip, float fov);
    void exportMatrix(Shader& shader, const char* uniform_name);
    void processInput(GLFWwindow* window);
    
    Camera(int width, int height, vec3 pos);
};
