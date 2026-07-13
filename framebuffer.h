#include "io.h"

/* define framebuffer address */
#define FB_ADDRESS 0x000B8000; 

/* The I/O port commands */
#define FB_COMMAND_PORT  0x3D4
#define FB_DATA_PORT 		 0x3D5

#define FB_HIGH_BYTE_COMMAND  14
#define FB_LOW_BYTE_COMMAND   15

#define FB_BLACK     0
#define FB_GREEN 		 2
#define FB_DARK_GREY 8

void fb_move_cursor(unsigned short pos);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_write_simple();
int  fb_write_str(char *buf, unsigned int len);
void fb_clear();
