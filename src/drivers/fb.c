#include <kernel/fb.h>
#include <kernel/io.h>

static u16 fb_cursor_pos = 0;

/* fb_move_cursor
 * Moves the hardware cursor to the given cell index.
 *
 * @param pos The new position of the cursor
 */
void fb_move_cursor(u16 pos) {
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT, (pos >> 8) & 0x00FF);
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT, pos & 0x00FF);
  fb_cursor_pos = pos;
}

/* fb_write_cell
 * Writes a character with the given foreground and background colour at byte
 * offset i in the framebuffer. Each cell is two bytes, so i is a byte offset,
 * not a cell index.
 *
 * @param i  The byte offset into the framebuffer
 * @param c  The character
 * @param fg The foreground colour
 * @param bg The background colour
 */
void fb_write_cell(u32 i, char c, u8 fg, u8 bg) {
  char *fb = (char *)FB_ADDRESS;
  fb[i] = c;
  fb[i + 1] = ((fg & 0x0F) << 4 | (bg & 0x0F));
}

/* fb_write_simple
 * Writes a single A to the framebuffer, green on grey.
 */
void fb_write_simple(void) { fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY); }

/* fb_write_str
 * Writes a null-terminated string at the cursor, advancing as it goes.
 *
 * @param  buf Array of characters to write
 * @return     Number of characters written
 */
int fb_write_str(char *buf) {
  u32 i = 0;
  while (buf[i] != '\0') {
    fb_write_cell(2 * fb_cursor_pos, buf[i], FB_BLACK, FB_GREEN);
    fb_move_cursor(fb_cursor_pos + 1);
    i++;
  }

  return i;
}

/* fb_clear
 * Blanks every cell and returns the cursor to the top left.
 */
void fb_clear(void) {
  u32 i = 0;
  while (i < FB_COLUMNS * FB_ROWS) {
    fb_write_cell(2 * i, ' ', FB_BLACK, FB_BLACK);
    i++;
  }
  fb_move_cursor(0);
}
