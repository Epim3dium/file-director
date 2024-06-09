#include "skybox.h"
#include "glm/gtc/type_ptr.hpp"
Skybox::Skybox(std::string filepath) {
    std::array<Bitmap, 6> bitmaps;
    std::string facesCubemap[6] =
    {
        filepath + "/right.jpg",
        filepath + "/left.jpg",
        filepath + "/top.jpg",
        filepath + "/bottom.jpg",
        filepath + "/front.jpg",
        filepath + "/back.jpg"
    };
    for(int i = 0; i < 6; i++) {
        bitmaps[i] = Texture::loadFromFile(facesCubemap[i].c_str(), false);
    }
    *this = Skybox(bitmaps);
}
Skybox::Skybox(std::array<Bitmap, 6> bitmaps) {
    shader().bind();
    glUniform1i(shader().u("skybox"), 0);


    unsigned int skyboxVBO, skyboxEBO;
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
        auto bmp = bitmaps[i];
        if (bmp.data) {
            if (bmp.channelNumber == 4)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
                        bmp.width, bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                        bmp.data);
            else if (bmp.channelNumber == 3)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
                        bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                        bmp.data);
            else if (bmp.channelNumber == 1)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
                        bmp.width, bmp.height, 0, GL_RED, GL_UNSIGNED_BYTE,
                        bmp.data);
            stbi_image_free(bmp.data);
        } else {
            std::cout << "Failed to load skybox\n";
        }
    }
}
void Skybox::decomission() {
	glDeleteTextures(1, &cubemapTexture);
}
void Skybox::draw(const Camera& cam) {
    glDepthFunc(GL_LEQUAL);


    shader().bind();
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
    // The last row and column affect the translation of the skybox (which we don't want to affect)
    view = glm::mat4(glm::mat3(glm::lookAt(cam.position, cam.position + cam.direction, cam.up)));
    projection = glm::perspective(glm::radians(45.0f), (float)cam.window_size.width / cam.window_size.height, 0.1f, 100.0f);

    glUniformMatrix4fv(shader().u("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader().u("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
    // where an object is present (a depth of 1.0f will always fail against any object's depth value)
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);
}
