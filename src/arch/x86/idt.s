	;       code section
	section .text
	global  idt_load

	; idt_load - loads the Interrupt Descriptor Table (IDT)
	; stack: [esp + 4] address of the struct idt_ptr
	; [esp    ] the return address

idt_load:
	mov  eax, [esp + 4]; load the address of the IDTR value into eax
	lidt [eax]; load the IDT
	ret  ; return to the calling function

%assign i 0
%rep    256
extern  interrupt_handler_%+i
%assign i i+1
%endrep

	;       rodata section
	section .rodata
	align   4
	global  interrupt_handler_table

interrupt_handler_table:
	%assign i 0
	%rep    256
	;       emits 32 bit address C sees it as plain array
	dd      interrupt_handler_%+i
	%assign i i+1
	%endrep
