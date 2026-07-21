	; load the Global Descriptor Table into the cpu

	global segments_load_gdt
	global segments_load_registers

	; this loads the gdt [esp + 4] is the struct (6 byte) passed as arg

segments_load_gdt:
	lgdt [esp + 4]
	ret

	; reload every segement register

segments_load_registers:
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	jmp 0x08:flush_cs; cs can only be changed by `far call` or iret

flush_cs:
	ret
