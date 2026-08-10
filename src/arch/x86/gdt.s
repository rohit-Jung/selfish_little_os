	; load the Global Descriptor Table into the cpu

	global gdt_load
	global gdt_reload_segments

	; gdt_load - loads the GDT
	; stack: [esp + 4] the 6-byte struct gdt_ptr, passed by value
	;        [esp    ] the return address

gdt_load:
	lgdt [esp + 4]
	ret

	; gdt_reload_segments - point every segment register at the new GDT
	; 0x10 = GDT index 2 (data), 0x08 = GDT index 1 (code)

gdt_reload_segments:
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	jmp 0x08:.flush_cs; cs can only be changed by a far jump, far call or iret

.flush_cs:
	ret
