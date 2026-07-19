#ifndef INCLUDE_INTERRUPTS
#define INCLUDE_INTERRUPTS

struct IDT {
  unsigned short size;
  unsigned int address;
} __attribute__((packed));

struct IDTDescriptor {
  // lowest 32 bit 
  unsigned short offset_low; // offset bits 0..15
  unsigned short segement_selector; // code segment selector in GDT or LDT
  
  // highest 32 bit
  unsigned char reserved; // 0 for now
  unsigned char type_and_attr;
  unsigned short offset_high; // offset bits 16..31
} __attribute__((packed));

void install_idt();

/* for asm */
void load_idt(unsigned int idt_address);
void interrupt_handler_33();
void interrupt_handler_14();

/** using the c code get state of register, stack and number of interrupts **/
struct cpu_state {
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;

	unsigned int esp;
} __attribute__((packed));

struct stack_state {
	unsigned int error_code;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
} __attribute__((packed));

#define INTERRUPTS_DESCRIPTOR_COUNT 256
#define INTERRUPTS_KEYBOARD         33
#define INTERRUPTS_PAGING           14

void interrupt_handler(struct cpu_state *cpu, struct stack_state *stack, unsigned int interrupt);

#endif
