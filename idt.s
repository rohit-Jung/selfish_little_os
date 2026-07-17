global load_idt

; load_idt - loads the Interrupt Descriptor table (IDT)
; stack: [esp + 4] address of the first entry in IDT
;        [esp    ] the return address

load_idt:
	mov  eax, [esp + 4]   ; load the address of IDT on the eax register
	lidt eax              ; load the IDT
	ret                   ; return to the calling function
