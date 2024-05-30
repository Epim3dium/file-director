#include"font_renderer.h"
#include "stb_truetype.h"
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
}
Texture FontRenderer::generate(std::string text, glm::vec3 color, GLuint slot) {
    int x = 0;
    
    std::vector<unsigned char> bitmap(m_width * m_height);
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
    return Texture(bitmap.data(), m_width, m_height, 1, "text", slot); 
}
