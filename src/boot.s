/* Multiboot constants */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    (ALIGN | MEMINFO)
.set MAGIC,    0x1BADB002       /* Multiboot magic number */
.set CHECKSUM, -(MAGIC + FLAGS)

/* Multiboot header */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Reserve space for the kernel stack (16 KB) */
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* Entry point - called by the bootloader */
.section .text
.global _start
.type _start, @function
_start:
    /* Set up the stack */
    mov $stack_top, %esp

    /* Clear the EFLAGS register (but preserve eax which contains the magic number) */
    push $0
    popf

    /* Save the Multiboot info pointer and magic number for the kernel */
    /* Push in reverse order for cdecl calling convention */
    push %ebx    /* Second parameter: mboot_ptr (pushed first) */
    push %eax    /* First parameter: magic (pushed second) */

    /* Call the C kernel entry point */
    call kernel_main

    /* If kernel_main returns, hang the CPU */
    cli
    hlt
    .byte 0xf4   /* Extra hlt for safety */
    jmp _start
