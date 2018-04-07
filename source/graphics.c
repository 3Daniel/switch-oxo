#include "graphics.h"

color_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    color_t clr;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    clr.a = a;
    return clr;
}

uint8_t BlendColor(uint32_t src, uint32_t dst, uint8_t alpha)
{
    uint8_t one_minus_alpha = (uint8_t)255 - alpha;
    return (dst*alpha + src*one_minus_alpha)/(uint8_t)255;
}

void DrawPixel(uint32_t x, uint32_t y, color_t clr)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    u32 off = (y * g_framebuf_width + x)*4;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.r, clr.a); off++;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.g, clr.a); off++;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.b, clr.a); off++;
    g_framebuf[off] = 0xff;
}

void DrawBox(uint32_t x, uint32_t y, uint32_t width, uint32_t height, color_t color)
{
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
	
	for(int i = x; i <= x + width; i++){
		if (i >= SCREEN_WIDTH)
			break;
		for(int j = y; j <= y + height; j++) {
			if (j >= SCREEN_HEIGHT)
				break;
			
			DrawPixel(i, j, color);
		}
	}
}