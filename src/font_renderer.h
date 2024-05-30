#ifndef FONT_RENDERER_CLASS_H
#define FONT_RENDERER_CLASS_H

#include "shader.h"
#include "defs.h"
#include "texture.h"
#include "stb_truetype.h"

class FontRenderer
{
    int m_width, m_height, m_line_height;
    float scale;
    int ascent;
    stbtt_fontinfo info;
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	FontRenderer(const char* font, int width, int height, int line_height);
    Texture generate(std::string text, glm::vec3 color, GLuint slot);
};
#endif
