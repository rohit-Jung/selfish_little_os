global  loader                   ; the entry symbol for ELF
extern  kmain

MAGIC_NUMBER  equ  0x1BADB002                        ; define the magic number constant
ALIGN_MODULES equ  0x00000001                        ; tell GRUB to align the modules
CHECKSUM      equ -(MAGIC_NUMBER + ALIGN_MODULES)    ; calculate the checksum
	                                                   ; (magic number + checksum + flags should equal 0)

section .text										  ; start of the text (code) section
align   4													  ; the code must be 4 byte aligned
	dd      MAGIC_NUMBER							; write the magic number to the machine code
	dd			ALIGN_MODULES							; write algin modules
	dd      CHECKSUM									; and the check sum

loader: 														; loader label defined as the entry point in script
	mov eax, 0xCAFEBABE               ; place the number 0xCAFEBABE in the eax register
  mov esp, kernel_stack_top         ; point esp to the top of created stack
	push ebx													; multiboot info parameter kmain args
  call kmain

.loop:
	jmp .loop                         ; loop forever

section .bss
align 4
  kernel_stack_bottom: resb 4096      ; 4KB stack C code needs stack
  kernel_stack_top:
