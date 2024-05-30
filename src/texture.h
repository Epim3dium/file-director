#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include"shader.h"
#include "defs.h"
#include <vector>

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);
	Texture(unsigned char* bitmap, int width, int height, int numberOfChannels, const char* texType, GLuint slot);

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
