#include <stdlib.h> /* for malloc */

struct FrambufferInfo{
    u32_t physical_width;
    u32_t physical_height;
    u32_t virtual_width;
    u32_t virtual_height;
    u32_t pitch;
    u32_t depth;
    u32_t x_offset;
    u32_t y_offset;
    u32_t pointer;  // is this ok for 64bit?
    u32_t size;     // nBytes - nBytes buffer has
}

struct rgb{
    char r;
    char g;
    char b;
}
struct rgb* s_pixels = 0;
size_t s_nPixels;
size_t s_nBytes;

struct FramebufferInfo*
AcquireFramebuffer(struct FramebufferInfo* pfbi)
{
    if (0 == s_pixels) {
        s_nPixels = pfbi->virtual_width * pfbi_virtual_height;
        s_nBytes = (pfbi->depth >> 3) * s_nPixels;
        s_pixels = malloc(s_nBytes);
    }
    pfbi->pitch   = (u32_t)0;         // 0 bytes inbetween rows
    pfbi->pointer = (u32_t) s_pixels; // ptr to pixel buffer
    pfbi->size    = (u32_t) s_nBytes;     
    return pfbi;
}

void
ReleaseFramebuffer()
{
    free(s_pixels);
    s_nPixels = 0;
    s_nBytes = 0;
}
