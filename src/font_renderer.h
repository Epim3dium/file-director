#ifndef FONT_RENDERER_CLASS_H
#define FONT_RENDERER_CLASS_H

#include "defs.h"
#include "texture.h"
#include "stb_truetype.h"

class FontRenderer
{
    stbtt_fontinfo info;
    std::string fontBuffer;
public:

	FontRenderer(const char* font);
    Texture generate(std::string text, int width, int height, GLuint slot, int font_height = 0, const char* type = "text");
    int getPixelWidth(std::string text, int font_height);
};
#endif
