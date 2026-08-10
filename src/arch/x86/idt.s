global idt_load

; idt_load - loads the Interrupt Descriptor Table (IDT)
; stack: [esp + 4] address of the struct idt_ptr
;        [esp    ] the return address

idt_load:
	mov  eax, [esp + 4]   ; load the address of the IDTR value into eax
	lidt [eax]            ; load the IDT
	ret                   ; return to the calling function
