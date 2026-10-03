#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

/* Fixed-width integer names.
 *
 * Kernel structs are laid out against hardware formats where the width of a
 * field *is* the contract. `unsigned short` says nothing; `u16` says exactly
 * what the CPU expects to read.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

#define NULL ((void *)0)

_Static_assert(sizeof(u8) == 1, "u8 must be 1 byte");
_Static_assert(sizeof(u16) == 2, "u16 must be 2 bytes");
_Static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
_Static_assert(sizeof(u64) == 8, "u64 must be 8 bytes");

#endif /* KERNEL_TYPES_H */
