#ifndef FONT_RENDERER_CLASS_H
#define FONT_RENDERER_CLASS_H

#include "shader.h"
#include "defs.h"
#include "texture.h"
#include "stb_truetype.h"

class FontRenderer
{
    stbtt_fontinfo info;
public:

	FontRenderer(const char* font);
    Texture generate(std::string text, glm::vec3 color, int width, int height, GLuint slot, int font_height = 0);
    int getPixelWidth(std::string text, int font_height);
};
#endif
