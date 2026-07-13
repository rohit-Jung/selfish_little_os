ENTRY(loader)  /* name of the entry label */

SECTIONS {
	. = 0x00100000; /* code should be loaded at 1MB */

	.text ALIGN (0x1000) :  /* align at 4KB */
	{
		*(.text)        /* all the text sections from file */
	}

	.rodata ALIGN (0x1000) :  /* align at 4KB */
	{
		*(.rodata*)        /* all the read only data sections from all files */
	}

	.data ALIGN (0x1000) :  /* align at 4KB */
	{
		*(.data)        /* all the data sections from all files */
	}

	.bss ALIGN (0x1000) :  /* align at 4KB */
	{
		*(COMMON)        /* all COMMON sections from all files */
		*(.bss)          /* all the bss sections from all files */
	}
}
