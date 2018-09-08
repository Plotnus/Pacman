
/* system & OS headers */
#include <fcntl.h>     /* for open */
#include <linux/fb.h>  /* for fb structs */
#include <sys/ioctl.h> /* for input output control */

/* cstdlib headers */
#include <stdio.h>     /* for printf */
#include <unistd.h>    /* for close  */
#include <string.h>    /* for memset */
#include <stdlib.h>    /* for malloc */


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

    // ALLOC
    void *displaybuffer = malloc(fb_fix_info.smem_len);
    if (NULL == displaybuffer) {
        printf("allocation of the framebuffer failed.\n");
        return 0;
    }
    memset(displaybuffer,0xFF,fb_fix_info.smem_len);        /* set to black                 */
    int nBytesPerRow = fb_fix_info.line_length;    /* length of a line in bytes    */

    int screen_xres = fb_var_info.xres_virtual;
    int screen_yres = fb_var_info.yres_virtual;
    int screen_npxl = screen_xres * screen_yres;

    // RENDER
    // set pacman gameboard to gray
    int board_xres = 244;
    int board_yres = 288;

    int border_npx_l = screen_xres - board_xres;
    for (int irow=0; irow<288; irow++) {
        // we need to write the corerct number of bytes
        int offset_px = (irow * screen_xres) + ((screen_xres - board_xres) >> 1);
        int offset_ch = offset_px * (fb_var_info.bits_per_pixel >> 3);
        void* prow = displaybuffer + offset_ch;
        memset(prow,0x88,(board_xres * (fb_var_info.bits_per_pixel >> 3)));
    }



    // DISPLAY
    lseek(fbfd,0,SEEK_SET);
    write(fbfd,displaybuffer,fb_fix_info.smem_len);


    // FREE
    free(displaybuffer);

    // SHUTDOWN
    close(fbfd);

    return 0;
}
