
/* system & OS headers */
#include <fcntl.h>     /* for open */
#include <linux/fb.h>  /* for fb structs */
#include <sys/ioctl.h> /* for input output control */
#include <sys/mman.h>  /* for mmap et al */

/* cstdlib headers */
#include <stdio.h>     /* for printf */
#include <unistd.h>    /* for close  */


int main(void) {
    printf("Hello C!\n");

    int fbfd; // Frame Buffer File Descriptior
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

    // map the memory - the new stuff
    void *pfb;
    pfb =  mmap(NULL ,fb_fixed_info.smem_len ,PROT_WRITE
            ,MAP_PRIVATE ,fbfd ,0);
    if (MAP_FAILED == pfb) {
        printf("failed to map framebuffer memory\n");
        return 0;
    }

    printf("Cool! we mapped the framebuffer to virtual address 0x%p\n",pfb);
    printf("This is the location we'll be writing pixels to!\n");

    munmap(pfb, fb_fixed_info.smem_len);

    return 0;
}
