#include"font_renderer.h"
#include "stb_truetype.h"
#include <iostream>
#include <vector>

FontRenderer::FontRenderer(const char* font) 
{
    fontBuffer = dumpStringFromFile(font);
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
    return (x / font_height + 1) * font_height;
}
Texture FontRenderer::generate(std::string text, int width, int height, GLuint slot, int font_height, const char* type) {
    auto bmp = generateBitmap(text, width, height, font_height);
    auto tex = Texture(bmp, type, slot);
    delete[] bmp.data;
    return tex;
}
Bitmap FontRenderer::generateBitmap(std::string text, int width, int height, int font_height) {
    int ascent, descent, lineGap;
    if(font_height == 0){
        std::cerr << "overloaded for: "<< text << "\n";
        font_height = height;
    }
    
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    float scale = stbtt_ScaleForPixelHeight(&info, font_height);
    ascent =  roundf(ascent * scale);
    descent = roundf(descent * scale);
    int x = 0;
    int y = 0;
    
    unsigned char* bitmap = new unsigned char[width * height] {0};
    for (int i = 0; i < text.size(); ++i)
    {
        int ax;
        int lsb;
        stbtt_GetCodepointHMetrics(&info, text[i], &ax, &lsb);
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        if(text[i] == '\t') {
            text[i] = ' ';
        }
        if(text[i] == '\n') {
            x = 0;
            y += font_height;
        }
        if(text[i] < ' ') {
            continue;
        }

        int yoff = ascent + c_y1 + y;
        if(yoff > height) {
            break;
        }
        if(x + roundf(ax * scale) > width) {
            y += font_height; 
            x = 0;
        }

        int byteOffset = x + roundf(lsb * scale) + (yoff * width);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, width, scale, scale, text[i]);

        x += roundf(ax * scale);

        if( i != text.size() - 1) {
            int kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
            x += roundf(kern * scale);
        }
    }
    std::cerr << "generated: " << text.substr(0, 32) << "\n";
    return {width, height, bitmap, 1}; 
}
