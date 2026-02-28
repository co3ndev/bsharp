ENTRY(_start)

SECTIONS
{
    /* The kernel will be loaded at this address */
    . = 0x100000;

    /* Align to page boundary (4KB) */
    . = ALIGN(4K);

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /* Discard unused sections */
    /DISCARD/ :
    {
        *(.eh_frame)
        *(.note.GNU-stack)
    }
}
