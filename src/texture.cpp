#include "texture.h"
#include <iostream>

Texture::Texture(Bitmap bmp, const char* texType, GLuint slot) {
    if(slot == 0) {
        std::cerr << "Default texture slot is being used!\n";
    }
	type = texType;
    m_width = bmp.width;
    m_height = bmp.height;
    int numColCh = bmp.channelNumber;

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);

	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (numColCh == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.width, bmp.height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, bmp.data);
    else if (numColCh == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.width, bmp.height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, bmp.data);
    else if (numColCh == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.width, bmp.height, 0,
                GL_RED, GL_UNSIGNED_BYTE, bmp.data);
    else
        throw std::invalid_argument(
                "Automatic Texture type recognition failed");
    glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}
Bitmap Texture::loadFromFile(const char* filename, bool flip_vert) {
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(flip_vert);
	unsigned char* bytes = stbi_load(filename, &widthImg, &heightImg, &numColCh, 0);
    if(bytes == nullptr) {
        std::cerr << "Failed to load image: " << filename << "\n";
    }else {
        std::cerr << "loaded: " << filename << "\n";
    }
    return {widthImg, heightImg, bytes, numColCh};
}
Texture::Texture(const char* image, const char* texType, GLuint slot)
{
    auto bmp = loadFromFile(image);
    *this = Texture(bmp, texType, slot);
    stbi_image_free(bmp.data);
}

void Texture::texUnit(Shader& shader, const char* uniform)
{
	GLuint texUni = shader.u(uniform);
	shader.bind();
	glUniform1i(texUni, unit);
}

void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::remove()
{
	glDeleteTextures(1, &ID);
}
