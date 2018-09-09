#include "pxbuffer.h"
#include <string.h>             /* for memset */
#include <stdlib.h>             /* for malloc and free */

/* ALLOC */
extern struct PxBuffer8*
PxBuffer8_Alloc(uint32_t xres, uint32_t yres)
{
    struct PxBuffer8 *pxb = (struct PxBuffer8 *) malloc(sizeof(struct PxBuffer8));
    pxb->xres = xres;
    pxb->yres = yres;
    pxb->size = xres * yres;
    pxb->data = (struct RGB8 *) malloc(pxb->size);
    return pxb;
}
extern void
PxBuffer8_Free(struct PxBuffer8* b)
{
    free(b->data);
    free(b);
    return;
}
//
extern struct PxBuffer32*
PxBuffer32_Alloc(uint32_t xres, uint32_t yres)
{
    struct PxBuffer32 *pxb = (struct PxBuffer32 *) malloc(sizeof(struct PxBuffer32));
    pxb->xres = xres;
    pxb->yres = yres;
    pxb->size = xres * yres * 4;
    pxb->data = (struct RGBA32 *) malloc(pxb->size);
    return pxb;
}
extern void
PxBuffer32_Free(struct PxBuffer32* b)
{
    free(b->data);
    free(b);
    return;
}
