#ifndef KERNEL_FB_H
#define KERNEL_FB_H

#include <kernel/types.h>

/* Start of VGA text-mode memory. Two bytes per cell: character, then
 * attribute. 80 columns by 25 rows. */
#define FB_ADDRESS 0x000B8000

#define FB_COLUMNS 80
#define FB_ROWS 25

/* The I/O port commands */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

#define FB_BLACK 0
#define FB_GREEN 2
#define FB_DARK_GREY 8

void fb_move_cursor(u16 pos);
void fb_write_cell(u32 i, char c, u8 fg, u8 bg);
void fb_write_simple(void);
int fb_write_str(char *buf);
void fb_clear(void);

#endif /* KERNEL_FB_H */
