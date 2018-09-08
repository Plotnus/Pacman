
/* system & OS headers */
#include <fcntl.h>     /* for open */
#include <linux/fb.h>  /* for fb structs */
#include <sys/ioctl.h> /* for input output control */

/* cstdlib headers */
#include <stdio.h>     /* for printf */
#include <unistd.h>    /* for close  */
#include <string.h>    /* for memset */
#include <stdlib.h>    /* for malloc */

/* our headers */
#include "pxbuffer.h" /* for PxBuffer */


int main(void) {

    int fbfd;
    struct fb_fix_screeninfo fb_fix_info;
    struct fb_var_screeninfo fb_var_info;

    fbfd = open("/dev/fb0",O_RDWR);
    if (0 == fbfd){
        printf("failed to open \"/dev/fb0\".\n");
        return 0;
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fb_fix_info)){
        printf("failed to get fixed fb info\n");
        return 0;
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &fb_var_info)){
        printf("failed to get variable fb info\n");
        return 0;
    }

    // STARTUP
    // allocate our buffers
    struct PxBuffer8  *game_PxBuffer    = PxBuffer8_Alloc(244,288);
    struct PxBuffer32 *display_PxBuffer = PxBuffer32_Alloc(fb_var_info.xres_virtual, fb_var_info.yres_virtual);

    // test code
    /* 1) set the display_PxBuffer to all black */
    memset(display_PxBuffer->data,0x00,display_PxBuffer->size);
    /* 2) set the game_PxBuffer */
    memset(game_PxBuffer->data,0b00111111,game_PxBuffer->size);
    
    {/* 3) set the display_PxBuffer based on the displayPxBuffer */
        struct RGB8  *gpx = game_PxBuffer->data;
        struct RGBA32 *dpx = display_PxBuffer->data;

            /* for _my_ monitor I know MAX(yscale,xscale) = yscale so can do scale = yscale*/
        printf("g->yres:%d\n",game_PxBuffer->yres);
        uint32_t scale = display_PxBuffer->yres  / game_PxBuffer->yres; 
        uint32_t sidemargin = (display_PxBuffer->xres - (game_PxBuffer->xres * scale)) >> 1;
        uint32_t topmargin = (display_PxBuffer->yres - (game_PxBuffer->yres * scale)) >> 1;

        /* this needs to be more cache friendly depending on the row size*/
        for (uint32_t irow=0; irow<game_PxBuffer->yres; ++irow) { /* y-axis */

            struct RGB8 *from_row = gpx + (game_PxBuffer->xres * irow);
            struct RGBA32 *to_row = dpx + sidemargin + (((irow*scale) + topmargin - 1) * display_PxBuffer->xres);
            for (uint32_t irepeatrow=0; irepeatrow<scale; ++irepeatrow) { /* y-axis scaling */
                to_row += display_PxBuffer->xres;
                for (uint32_t icol=0; icol<game_PxBuffer->xres; icol++) { /* x-axis */
                    /* could this be faster?*/
                    /* we have to reinitialize the following data scale times */
                    /* those could be cache misses and really slow the system down */
                    /* inspect and test this later */
                    
                    struct RGB8 *from = from_row + icol;
                    struct RGBA32 *to = to_row + (icol * scale);

                    to->data = 0;
                    to->a = 0xFF;
                    to->r += 0x21 & ~(from->r_l - 1);
                    to->r += 0x47 & ~(from->r_m - 1);
                    to->r += 0x97 & ~(from->r_h - 1);

                    to->g += 0x21 & ~(from->g_l - 1);
                    to->g += 0x47 & ~(from->g_m - 1);
                    to->g += 0x97 & ~(from->g_h - 1);

                    to->b += 0x51 & ~(from->b_l - 1);
                    to->b += 0xAE & ~(from->b_h - 1);
                    for (int irepeat =1; irepeat<scale; irepeat++) { /* x-axis scaling*/
                        struct RGBA32 *repeatpx = to + irepeat;
                        repeatpx->data = to->data;
                    }
                }
            }
        }
    }

    // DISPLAY
    lseek(fbfd,0,SEEK_SET);
    write(fbfd,display_PxBuffer->data,display_PxBuffer->size);


    // FREE
    PxBuffer8_Free(game_PxBuffer);
    PxBuffer32_Free(display_PxBuffer);

    // SHUTDOWN
    close(fbfd);

    return 0;
}
