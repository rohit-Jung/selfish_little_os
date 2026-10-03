extern isr_dispatch

	; macro feature on NASM

	; macros declare global labels the addresses of the interrupt handlers to be accessed from C
	; or assembly while creating IDT

	;      no error code interrupt handler
	%macro no_error_code_interrupt_handler 1
	global interrupt_handler_%+ %1

interrupt_handler_%+ %1:
	push dword 0; push 0 as error code
	push dword %1; push the interrupt number
	jmp  common_interrupt_handler; jump to the common interrupt handler
%endmacro

	;      error code interrupt handler
	%macro error_code_interrupt_handler 1
	global interrupt_handler_%+ %1

interrupt_handler_%+ %1:
	push dword %1; push the interrupt number
	jmp  common_interrupt_handler; jump to common interrupt handler
%endmacro

	; common interrupt handler

common_interrupt_handler:
	; save the registers (pusha order matches struct cpu_state)
	pusha

	; Build arguments on the stack (cdecl: right-to-left)
	; At this point
	; esp+0  = edi (start of cpu_state)
	; esp+28 = eax (end of cpu_state)
	; esp+32 = interrupt number
	; esp+36 = error code (start of stack_state)
	; esp+40 = eip
	; esp+44 = cs
	; esp+48 = eflags

	push dword [esp + 32]; interrupt number
	lea  eax, [esp + 40]; pointer to the stack_state (error code)
	push eax
	lea  eax, [esp + 8]; pointer to cpu_state (saved edi)
	push eax

	;    call the c function
	call isr_dispatch

	;   clean up 3 args
	add esp, 12

	; restore the registers
	popa

	;   drop interrupt number and error code
	add esp, 8

	; return to the code that got interrupted
	iret

	%assign i 0
	%rep    256
	%if     i == 8 || i == 10 || i == 11 || i == 12 || i == 13 || i == 14 || i == 17
	error_code_interrupt_handler i
	%else
	no_error_code_interrupt_handler i
	%endif
	%assign i i+1
	%endrep

	; no_error_code_interrupt_handler 0; handler for interrupt 0
	; no_error_code_interrupt_handler 1; handler for interrupt 1

	; error_code_interrupt_handler 7; handler for interrupt 7

	; no_error_code_interrupt_handler 33
	; error_code_interrupt_handler 14
