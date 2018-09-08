
/* system & OS headers */
#include <fcntl.h>     /* for open */
#include <linux/fb.h>  /* for fb structs */
#include <sys/ioctl.h> /* for input output control */

/* cstdlib headers */
#include <stdio.h>     /* for printf */
#include <unistd.h>    /* for close, usleep */
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
    int nfb = 2;
    int idb = 0;  // Index of Display Buffer
    int irb = 1;  // Index of Render Buffer
    char *pfb[nfb];
    pfb[0] = malloc(fb_fixed_info.smem_len);
    if (NULL == pfb[0]) {
        printf("allocation of the framebuffer0 failed.\n");
        goto EXIT;
    }
    pfb[1] = malloc(fb_fixed_info.smem_len);
    if (NULL == pfb[1]) {
        printf("allocation of the framebuffer1 failed.\n");
        goto EXIT;
    }
    

    // RENDER
    char values[3];
    values[0] = 0xFF;
    values[1] = 0xAA;
    values[2] = 0x00;

    int ival = 0;
    size_t nbytes = fb_fixed_info.smem_len;
    for (int i=0; i<124; i++) {
        char* pdb = pfb[idb];  // display buffer
        char* prb = pfb[irb];   // render buffer
        char val  = values[ival];

        // calculate the buffer for next frames display
        memset(prb,val,nbytes);

        // write the previous frame buffer to display
        lseek(fbfd,0,SEEK_SET);
        write(fbfd,pdb,nbytes);

        // wait so we can see it
        useconds_t duration = 250 * 1000; // quarter of a second
        usleep(duration);

        idb = (idb + 1) % nfb;
        irb = ++irb % nfb;
        ival = (ival + 1) % 3;

    }

EXIT:
    // FREE
    free(pfb[0]);
    free(pfb[1]);

    // SHUTDOWN
    close(fbfd);

    return 0;
}
