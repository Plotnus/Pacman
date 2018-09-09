#pragma once
#include <stddef.h>         /* for size_t */
#include <stdint.h>         /* for all the uint8_t, et al */

struct RGBA32{
    union {
        struct {
            // ordering is important so can be written to FB correctly
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        };
        uint32_t data;
    };
};
struct RGB8 {
    union {
        struct {
            uint8_t r_l : 1;  // l,m,h == low,medium,high
            uint8_t r_m : 1;
            uint8_t r_h : 1;
            uint8_t g_l : 1;
            uint8_t g_m : 1;
            uint8_t g_h : 1;
            uint8_t b_l : 1;
            uint8_t b_h : 1;
        };
        struct {
            uint8_t r : 3;
            uint8_t g : 3;
            uint8_t b : 2;
        };
        uint8_t data;
    };
};
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct PxBuffer8 {
    uint32_t xres;
    uint32_t yres;
    struct RGB8 *data;
    size_t size;            /* size of data in bytes */
};
struct PxBuffer32 {
    uint32_t xres;
    uint32_t yres;
    struct RGBA32 *data;
    size_t size;            /* size of data in bytes */
};

extern struct PxBuffer8*
PxBuffer8_Alloc(uint32_t xres, uint32_t yres);
extern struct PxBuffer32*
PxBuffer32_Alloc(uint32_t xres, uint32_t yres);

extern void
PxBuffer8_Free(struct PxBuffer8* b);
extern void
PxBuffer32_Free(struct PxBuffer32* b);




