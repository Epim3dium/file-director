#include"font_renderer.h"
#include "stb_truetype.h"
#include <iostream>
#include <vector>

FontRenderer::FontRenderer(const char* font) 
{
    std::string fontBuffer = dumpStringFromFile(font);
    if (!stbtt_InitFont(&info, (unsigned char*)fontBuffer.data(), 0))
    {
        printf("failed\n");
    }
}
int FontRenderer::getPixelWidth(std::string text, int font_height) {
    int ascent, descent, lineGap;
    
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    float scale = stbtt_ScaleForPixelHeight(&info, font_height);
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    int x = 0;
    for (int i = 0; i < text.size(); ++i)
    {
        int ax;
        int lsb;
        stbtt_GetCodepointHMetrics(&info, text[i], &ax, &lsb);

        x += roundf(ax * scale);

        // if( i != text.size() - 1) {
        //     int kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
        //     x += roundf(kern * scale);
        // }
    }
    return x;
}
Texture FontRenderer::generate(std::string text, glm::vec3 color, int width, int height, GLuint slot, int font_height) {
    int ascent, descent, lineGap;
    if(font_height == 0){
        font_height = height;
    }
    
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    float scale = stbtt_ScaleForPixelHeight(&info, font_height);
    ascent =  roundf(ascent * scale);
    descent = roundf(descent * scale);
    int x = 0;
    int y = 0;
    
    std::vector<unsigned char> bitmap(width * height );
    for (int i = 0; i < text.size(); ++i)
    {
        int ax;
        int lsb;
        stbtt_GetCodepointHMetrics(&info, text[i], &ax, &lsb);
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        int yoff = ascent + c_y1 + y;

        int byteOffset = x + roundf(lsb * scale) + (yoff * width);
        stbtt_MakeCodepointBitmap(&info, bitmap.data() + byteOffset, c_x2 - c_x1, c_y2 - c_y1, width, scale, scale, text[i]);

        x += roundf(ax * scale);

        if( i != text.size() - 1) {
            int kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
            x += roundf(kern * scale);
        }
        if(x > width) {
            y += font_height; 
            x = 0;
        }
    }
    std::vector<unsigned char> bitmap_colored(width * height*3);
    for(int i = 0; i < bitmap.size(); i++) {
        bitmap_colored[i*3 + 0] = bitmap[i] * color.r;
        bitmap_colored[i*3 + 1] = bitmap[i] * color.g;
        bitmap_colored[i*3 + 2] = bitmap[i] * color.b;
    }
    return Texture(bitmap_colored.data(), width, height, 3, "text", slot); 
}
