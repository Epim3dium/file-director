#include <iostream>
#include <filesystem>
#include <thread>
#include "defs.h"
#include "shader.h"
#include "camera.h"
#include "folder.hpp"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;


float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};
unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

Folder folder;
Shader spLambert;
Shader spSkybox;
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
void drawFolder(glm::mat4 Mt) {
    spLambert.bind();
	glUniform4f(spLambert.u("color"), 0, 1, 0, 1);
    glUniformMatrix4fv(spLambert.u("M"), 1, false, glm::value_ptr(Mt));
    cam.exportMatrix(spLambert, "cam_mat");
    folder.draw();
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


    folder.init();
    spLambert.init("../assets/v_lambert.glsl", "../assets/f_lambert.glsl");
    spSkybox.init("../assets/v_skybox.glsl", "../assets/f_skybox.glsl");
    spSkybox.bind();
	glUniform1i(spSkybox.u("skybox"), 0);

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    std::string facesCubemap[6] =
	{
		"../assets/skybox/right.jpg",
		"../assets/skybox/left.jpg",
		"../assets/skybox/top.jpg",
		"../assets/skybox/bottom.jpg",
		"../assets/skybox/front.jpg",
		"../assets/skybox/back.jpg"
	};
    unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}
    
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
        drawFolder(Mk);
        
        glDepthFunc(GL_LEQUAL);

		spSkybox.bind();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(cam.position, cam.position + cam.direction, cam.up)));
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        
		glUniformMatrix4fv(spSkybox.u("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(spSkybox.u("projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);

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
