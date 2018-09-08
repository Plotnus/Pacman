
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
    struct fb_fix_screeninfo fb_fixed_info;
    struct fb_var_screeninfo fb_variable_info;

    fbfd = open("/dev/fb0",O_RDWR);
    if (0 == fbfd){
        printf("failed to open \"/dev/fb0\".\n");
        return 0;
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fb_fixed_info)){
        printf("failed to get fixed fb info\n");
        return 0;
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &fb_variable_info)){
        printf("failed to get variable fb info\n");
        return 0;
    }

    // ALLOC
    void *pfb = malloc(fb_fixed_info.smem_len);
    if (NULL == pfb) {
        printf("allocation of the framebuffer failed.\n");
        return 0;
    }

    // RENDER
    memset(pfb,0xFF,fb_fixed_info.smem_len);

    // DISPLAY
    write(fbfd,pfb,fb_fixed_info.smem_len);

    // FREE
    free(pfb);

    // SHUTDOWN
    close(fbfd);

    return 0;
}
