#pragma once
#include <iostream>
#include <filesystem>
#include <thread>
#include "defs.h"
#include "shader.h"
#include "camera.h"

static const float skyboxVertices[] =
{
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
};
static const unsigned int skyboxIndices[] =
{
	// right
	1, 2, 6,
	6, 5, 1,
	// left
	0, 4, 7,
	7, 3, 0,
	// top
	4, 5, 6,
	6, 7, 4,
	// bottom
	0, 3, 2,
	2, 1, 0,
	// back
	0, 1, 5,
	5, 4, 0,
	// front
	3, 7, 6,
	6, 2, 3
};
#define SKYBOX_VERTEX_SHADER "../assets/v_skybox.glsl"
#define SKYBOX_FRAGMENT_SHADER "../assets/f_skybox.glsl"
class Skybox {
private:
    static Shader& shader() {
        static Shader shader(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER);
        return shader;
    }
    unsigned int skyboxVAO;
    unsigned int cubemapTexture;
public:
    void draw(const Camera& cam);
    Skybox(std::string filepath);
};
