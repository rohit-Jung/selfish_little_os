#include "io.h"
#include "serial.h"
#include "framebuffer.h"

int kmain()
{
	fb_move_cursor(6*80);
	fb_write_simple();
	return 0;
}


