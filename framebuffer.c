#include "framebuffer.h"
#include "io.h"

unsigned short __fb_present_pos = 0x00000000;

/**
 * fb_move_cursor
 * Moves the cursor to the given position in frame buffer
 *
 * @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos) {
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT, pos & 0x00FF);
  __fb_present_pos = pos;
}

/* fb_write_cell
 * Writes a character with given foreground and background to position i
 * in the frame buffer
 *
 * @param i  The location in frame buffer
 * @param c  The character
 * @param fg The foreground color
 * @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
  char *fb = (char *)FB_ADDRESS;
  fb[i] = c;
  fb[i + 1] = ((fg & 0x0F) << 4 | (bg & 0x0F));
}

/* fb_write_simple
 * Writes a simple A to the frame buffer with Green BG and Grey FB
 */
void fb_write_simple() 
{ 
	fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY); 
}

/* fb_write_str
 * Writes the given string on to the frame buffer
 * 
 * @param   *buf  Array of characters / string to write
 * @param   len    Length of the string
 * @returns len   Length printed on frame buffer
 */
int fb_write_str(char *buf, unsigned int len) 
{ 
	unsigned int i = 0;
	for (; i < len; i++) {
		fb_write_cell(2*__fb_present_pos, buf[i], FB_GREEN, FB_DARK_GREY);
		fb_move_cursor(__fb_present_pos + 1);
	}

	return i;
}

/* fb_clear
 * Clears the frame buffer by writing the ' ' throughout the buffer
 * 80 rows 25col
 */
void fb_clear() 
{
	unsigned int i = 1;
	while(i < 80*25*2) {
		fb_write_cell(2*i, ' ', FB_BLACK, FB_BLACK);
		i++;
	}

	return;
}


