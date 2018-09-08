
/* system & OS headers */
#include <fcntl.h>     /* for open */
#include <linux/fb.h>  /* for fb structs */
#include <sys/ioctl.h> /* for input output control */
#include <sys/mman.h>  /* for mmap et al */

/* cstdlib headers */
#include <stdio.h>     /* for printf */
#include <string.h>    /* for memset */
#include <unistd.h>    /* for close  */

typedef unsigned rgba;

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

    /* print info about the fixed fb */
    printf("FIXED FB INFO:\n");
    printf("\tid:%s\n",fb_fixed_info.id);
    printf("\tmmio_start:0x%08lx\n", fb_fixed_info.mmio_start);
    printf("\tmmio_len:%d\n", fb_fixed_info.mmio_len);

    printf("\n");
    printf("VARIABLE FB INFO:\n");
    printf("\tres_x:%d\n",fb_variable_info.xres);
    printf("\tres_y:%d\n",fb_variable_info.yres);
    printf("\tbits per pixel:%d\n",fb_variable_info.bits_per_pixel);

    return 0;
}
