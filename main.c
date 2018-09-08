
/* system & OS headers */
#include <fcntl.h>     /* for open */
#include <linux/fb.h>  /* for fb structs */
#include <sys/ioctl.h> /* for input output control */
#include <sys/mman.h>  /* for mmap et al */

/* cstdlib headers */
#include <stdio.h>     /* for printf */
#include <string.h>    /* for memset */
#include <unistd.h>    /* for close  */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Standard Types */
#define u32_t unsigned int
#define s32_t int


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef unsigned rgba;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* DEFINED IM memset.s */
extern void* memset4(void* s, u32_t v, size_t n);
extern void _ASM_BEGIN();
 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define FB_INVALID 0
#define FB_ERR_SUCCESS 0
#define FB_ERR_INIT_OPEN 1
#define FB_ERR_INIT_VAR_SCREEN_INFO 2
#define FB_ERR_INIT_FIXED_SCREEN_INFO 3
#define FB_ERR_NO_INIT 4
#define FB_ERR_INIT_MMAP 5

static int s_fbfd = {0};
static int s_err = FB_ERR_NO_INIT;
static struct fb_fix_screeninfo s_fbInfoF = {0}; /* F for varies */
static struct fb_var_screeninfo s_fbInfoV = {0}; /* V for varies */
static void* s_fbMappedMemStart;

int FB_Init()
{
    s_fbfd = open("/dev/fb0",O_RDWR);
    if (FB_INVALID == s_fbfd){
        printf("failed to open \"/dev/fb0\".\n");
        s_err = FB_ERR_INIT_OPEN;
        return  FB_ERR_INIT_OPEN;

    }

    if (ioctl(s_fbfd, FBIOGET_FSCREENINFO, &s_fbInfoF)){
        printf("failed to get fixed fb info\n");
        s_err = FB_ERR_INIT_FIXED_SCREEN_INFO;
        return  FB_ERR_INIT_FIXED_SCREEN_INFO;
    }

    if (ioctl(s_fbfd, FBIOGET_VSCREENINFO, &s_fbInfoV)){
        printf("failed to get variable fb info\n");
        s_err = FB_ERR_INIT_VAR_SCREEN_INFO;
        return  FB_ERR_INIT_VAR_SCREEN_INFO;
    }

    // map the FB to memory we can set
    s_fbMappedMemStart = (char*)mmap(0
        ,s_fbInfoF.smem_len
        ,(PROT_READ | PROT_WRITE)
        ,MAP_SHARED
        ,s_fbfd
        ,0
    );

    if (s_fbMappedMemStart == (void*)0xFFFFFFFF){
        printf("failed to map FB memory\n");
        s_err = FB_ERR_INIT_MMAP;
        return  FB_ERR_INIT_MMAP;
    }

    s_err = FB_ERR_SUCCESS;
    return FB_ERR_SUCCESS;
}

void FB_Shutdown()
{
    munmap(s_fbMappedMemStart,s_fbInfoF.smem_len);
    close(s_fbfd);
}

void* FB_GetAddr()
{
    printf("FB_GetAddr:0x%p\n",s_fbMappedMemStart);
    if (s_err == FB_ERR_SUCCESS){
        return s_fbMappedMemStart;
    }
    return NULL;
}
size_t FB_GetNumBytes()
{
    return s_fbInfoF.smem_len;
}
void FB_PrintFixedInfo()
{
    /* print info about the fixed fb */
    printf("FIXED FB INFO:\n");
    printf("\tid:%s\n",s_fbInfoF.id);
    printf("\tmmio_start:0x%08lx\n", s_fbInfoF.mmio_start);
    printf("\tmmio_len:%d\n", s_fbInfoF.mmio_len);
    return;
}
void FB_PrintVarInfo()
{
    printf("\n");
    printf("VARIABLE FB INFO:\n");
    printf("\tres_x:%d\n",s_fbInfoV.xres);
    printf("\tres_y:%d\n",s_fbInfoV.yres);
    printf("\tbits per pixel:%d\n",s_fbInfoV.bits_per_pixel);
    return;
}
    
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
u32_t Color(char r, char g, char b)
{
    u32_t c = 0;
    c |= 0xFF;
    c <<= 8;
    c |= r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    printf("color:0x%08x\n",c);
    return c;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int main(void) {
    printf("Hello C!\n");

    FB_Init();


    FB_PrintFixedInfo();
    FB_PrintVarInfo();
    void* fb = FB_GetAddr();

    size_t nBytes = FB_GetNumBytes();
    size_t nWords = nBytes/4;

    int kColors = 13;
    int nWordsPerColor = nWords/kColors; // div by 16

    // written as {A,R,G,B} because of endianess
    rgba color[kColors];

    rgba *c = color-1;
//  *(++c) = Color(0,0,0);
//  *(++c) = 0xFF000000;//{0,0,0}           // BLACK
//  *(++c) = Color(255,0,255);
//  *(++c) = 0xFF0000FF;//{255,0,255}       // BLUE
//  *(++c) = Color(221,151,81);
//  *(++c) = 0xFF51A1FF;//{222,151,81}      // 
//  *(++c) = Color(0,255,0);
//  *(++c) = 0xFF00FF00;//{0,255,0}         // GREEN
//  *(++c) = Color(255,184,255);
//  *(++c) = 0xFFFFB8FF;//{255,184,255}     // PINK
//  *(++c) = Color(222,222,255);
//  *(++c) = 0xFFFFCECE;//{222,222,255}     // TAN
//  *(++c) = Color(0,255,255);
//  *(++c) = 0xFFFFFF00;//{0,255,255}       // YELLOW
//  *(++c) = Color(71,184,174);
//  *(++c) = 0xFFAEB847;//{71,184,174}      // BROWN
//  *(++c) = Color(255,184,81);
//  *(++c) = 0xFF51B8FF;//{255,184,81}      //  
//  *(++c) = Color(255,255,0);
//  *(++c) = 0xFF00FFFF;//{255,255,0}       // RED
//  *(++c) = Color(33,33,255);
//  *(++c) = 0xFFFF2121;//{33,33,255}       // 
//  *(++c) = Color(255,184,174);
//  *(++c) = 0xFFAEB8FF;//{255,184,174}     // puke
/*
    *(++c) = Color(0,0,0);
    *(++c) = Color(255,0,0);
    *(++c) = Color(222,151,81);
    *(++c) = Color(255,184,255);
    *(++c) = Color(0,0,0);
    *(++c) = Color(0,255,255);
    *(++c) = Color(71,184,255);
    *(++c) = Color(255,184,81);

    *(++c) = Color(0,0,0);
    *(++c) = Color(255,255,0);
    *(++c) = Color(0,0,0);
    *(++c) = Color(33,33,255);
    *(++c) = Color(0,255,0);
    *(++c) = Color(71,184,174);
    *(++c) = Color(255,184,174);
    *(++c) = Color(222,222,255);
*/
    *(++c) = Color(0,0,0);      // BK - black

    *(++c) = Color(255,0,0);    // RD
    *(++c) = Color(222,151,81); // BR
    *(++c) = Color(255,184,81);
    *(++c) = Color(255,255,0);  // YL

    *(++c) = Color(0,255,0);    // GR
    *(++c) = Color(71,184,255);
    *(++c) = Color(0,255,255);
    *(++c) = Color(33,33,255);  // BL

    *(++c) = Color(71,184,174);
    *(++c) = Color(255,184,174);
    *(++c) = Color(255,184,255);
    *(++c) = Color(222,222,255);




    _ASM_BEGIN();

//  u32_t *buf = (u32_t*) fb;
//  
//  for (int iCol=0; iCol < kColors; iCol++){
//      memset4(buf, color[iCol], nWordsPerColor);
//      buf += nWordsPerColor;
//  }
    
    FB_Shutdown();

    return 0;
}

