#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include"shader.h"
#include "defs.h"
#include <vector>

struct Bitmap {
    int width;
    int height;
    unsigned char* data;
    int channelNumber;
};
class Texture
{
    int m_width, m_height;
public:
	GLuint ID;
	const char* type;
	GLuint unit;

    static Bitmap loadFromFile(const char* image, bool flip_vert = true);
	Texture(const char* image, const char* texType, GLuint slot);
	Texture(Bitmap map, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform);
	// Binds a texture
	void bind() const;
	// Unbinds a texture
	void unbind() const;
	// Deletes a texture
	void remove();
};
#endif
