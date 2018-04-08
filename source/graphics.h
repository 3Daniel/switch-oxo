#include <malloc.h>
#include <switch.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef union {
    uint32_t abgr;
    struct {
        uint8_t r,g,b,a;
    };
} color_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    color_t* pixels;
} Bitmap;

extern uint8_t* g_framebuf;
extern u32 g_framebuf_width;

color_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint8_t BlendColor(uint32_t src, uint32_t dst, uint8_t alpha);
void DrawPixel(uint32_t x, uint32_t y, color_t clr);
void DrawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, color_t color);
void DrawBitmap(int x, int y, Bitmap bmp);
int LoadBitmapImage(void *in_buff, Bitmap *out_image);