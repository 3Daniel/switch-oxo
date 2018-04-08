#include "graphics.h"

#define BMP_SIG	0x4D42

typedef struct
{
	uint16_t signature; // BMP_SIG
	uint32_t filesize;
	uint16_t reserved[2]; // 0
	uint32_t imgstart;
	uint32_t headsize;
	uint32_t width;
	uint32_t height;
	uint16_t planes; // 1
	uint16_t bpp; // 1, 4, 8, 24, 32
	uint32_t compres; // 0
	uint32_t imgsize;
	uint32_t etc[4];
} __attribute__((packed)) bmphead_t;

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

void DrawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, color_t color)
{
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
	
	for(int i = x; i <= x + width; i++){
		if (i >= SCREEN_WIDTH)
			continue;
		for(int j = y; j <= y + height; j++) {
			if (j >= SCREEN_HEIGHT)
				break;
			
			DrawPixel(i, j, color);
		}
	}
}

void DrawBitmap(int x, int y, Bitmap bmp) {
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
	
	for (int i = 0; i < bmp.width; i++) {
		if (x + i >= SCREEN_WIDTH)
			break;
		
		for (int j = 0; j < bmp.height; j++) {
			if (y + j >= SCREEN_HEIGHT)
				break;
			
			int pos = i + bmp.width * (bmp.height - j);
			DrawPixel(x + i, y + j, bmp.pixels[pos]);
		}
	}
}

int LoadBitmapImage(void *in_buff, Bitmap *out_image)
{
	bmphead_t *head = in_buff;

	//Verify bitmap header
	if(head->signature != BMP_SIG || head->headsize > 1024 || head->planes != 1 || head->compres || head->imgstart > 2048)
	{
		return 1;
	}
	
	out_image->width = head->width;
	out_image->height = head->height;
	out_image->pixels = malloc((out_image->width * out_image->height) * sizeof(color_t));
	
	uint8_t *image = in_buff; 
	
	int pos = head->imgstart;
	
	for (int i = 0; i < (out_image->width * out_image->height); i++) {
		out_image->pixels[i] = Color(image[pos + 2], image[pos + 1], image[pos + 0], image[pos + 3]);
		pos = pos + 4;
	}

	return 0;
}