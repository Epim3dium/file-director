#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "glm/glm.hpp"
#include "shader.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

const unsigned int NUMBER_OF_VERTICES = 7;
const unsigned int VERTEX_SIZE = 2;
const float VERTICES[NUMBER_OF_VERTICES * VERTEX_SIZE] = {
        0.0f, 0.0f,
        -0.5f, 0.5f,
        -0.2f, 0.5f,
        -0.1f, 0.4f,
        0.5f, 0.4f,
        0.5f, -0.5f,
        -0.5f, -0.5f
};

const unsigned int TRIANGLE_COUNT = 6;
const unsigned int NUMBER_OF_INDICES = 3 * TRIANGLE_COUNT;
const unsigned int INDICES[NUMBER_OF_INDICES] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1
};

void onKeyPressed(GLFWwindow *window, int key, int scanCode, int action, int mode) {
    std::cout << "Key pressed [" << key << "]" << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        std::cout << "Escape pressed. Window should close." << std::endl;
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

bool isGLError() {
    while (GLenum error = glGetError()) {
        std::cout << "GL error [" << error << "]" << std::endl;
        return true;
    }
    return false;
}

int main() {
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Compute projection matrix
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Compute view matrix
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

    std::cout << "Initializing GLAD" << std::endl;
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Could not initialize GLAD\n");
        return -1;
    }

    std::cout << "Graphics context:" << std::endl;
    std::cout << "-- Vendor " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "-- Renderer " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "-- Version " << glGetString(GL_VERSION) << std::endl;

    unsigned int vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

    unsigned int vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, VERTEX_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), nullptr);

    unsigned int indexBufferId;
    glGenBuffers(1, &indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

    Shader shader("../assets/v_basic.glsl", "../assets/f_basic.glsl");
    shader.Bind();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, NUMBER_OF_INDICES, GL_UNSIGNED_INT, nullptr);

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
