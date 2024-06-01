#include"font_renderer.h"
#include "stb_truetype.h"
#include <iostream>
#include <vector>

FontRenderer::FontRenderer(const char* font, int width, int height, int line_height) : m_width(width), m_height(height), m_line_height(line_height)
{
    std::string fontBuffer = dumpStringFromFile(font);
    if (!stbtt_InitFont(&info, (unsigned char*)fontBuffer.data(), 0))
    {
        printf("failed\n");
    }
    int descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    scale = stbtt_ScaleForPixelHeight(&info, m_line_height);
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    std::cerr << ascent << "\n";
    std::cerr << descent << "\n";
}
Texture FontRenderer::generate(std::string text, glm::vec3 color, GLuint slot) {
    int x = 0;
    
    std::vector<unsigned char> bitmap(m_width * m_height );
    for (int i = 0; i < text.size(); ++i)
    {
        int ax;
        int lsb;
        stbtt_GetCodepointHMetrics(&info, text[i], &ax, &lsb);

        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        int y = ascent + c_y1;

        int byteOffset = x + roundf(lsb * scale) + (y * m_width);
        stbtt_MakeCodepointBitmap(&info, bitmap.data() + byteOffset, c_x2 - c_x1, c_y2 - c_y1, m_width, scale, scale, text[i]);

        x += roundf(ax * scale);

        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
        x += roundf(kern * scale);
    }
    std::vector<unsigned char> bitmap_colored(m_width * m_height*3);
    for(int i = 0; i < bitmap.size(); i++) {
        bitmap_colored[i*3 + 0] = bitmap[i] * color.r;
        bitmap_colored[i*3 + 1] = bitmap[i] * color.g;
        bitmap_colored[i*3 + 2] = bitmap[i] * color.b;
    }
    return Texture(bitmap_colored.data(), m_width, m_height, 3, "text", slot); 
}
