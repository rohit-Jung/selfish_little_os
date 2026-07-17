; macro feature on NASM

; macros declare global labels the addresses of the interrupt handlers to be accessed from C
; or assembly while creating IDT

; no error code interrupt handler
%macro no_error_code_interrupt_handler %1
global interrupt_handler_%1
interrupt_handler_%1:
	push dword 0                  ; push 0 as error code
  push dword %1                 ; push the interrupt number
 	jmp  common_interrupt_handler ; jump to the common interrupt handler
%endmacro


; error code interrupt handler
%macro error_code_interrupt_handler %1
global interrupt_handler_%1
interrupt_handler_%1:
	push dword %1                  ; push the interrupt number
	jmp  common_interrupt_handler  ; jump to common interrupt handler
%endmacro

; common interrupt handler
common_interrupt_handler:
	; save the registers
	push eax
	push ebx
	push ecx
	push edx
	push ebp
	push esi
	push edi

 	; call the c function
	call interrupt_handler

	; restore the registers
	pop edi
	pop esi
	pop ebp
	pop edx
	pop ecx
	pop ebx
	pop eax

  ; restore the esp 
	add esp, 8

	; return to the code that got interrupted
	iret

no_error_code_interrupt_handler 0   ; handler for interrupt 0
no_error_code_interrupt_handler 1   ; handler for interrupt 1

error_code_interrupt_handler 7      ; handler for interrupt 7
