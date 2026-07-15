#include "io.h"
#include "serial.h"
#include "framebuffer.h"

int kmain()
{
	char message[] = "Sleep rokshh";

  fb_clear();
	fb_move_cursor(6*80);
	/* fb_write_simple(); */
	fb_write_str(message);
  serial_write_str(message);
	return 0;
}


