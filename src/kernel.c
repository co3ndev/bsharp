#include <stdint.h>
#include "drivers/vga-display/vga_display.h"

/* Multiboot information structure (simplified) */
struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
} __attribute__((packed));

/* Main kernel entry point */
void kernel_main(uint32_t magic, struct multiboot_info *mboot_ptr) {
    /* Initialize VGA display driver */
    vga_init();

    /* Print welcome message */
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_puts("=== bsharp bootloader ===\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    vga_puts("[OK] Bootloader initialized successfully!\n\n");

    /* Display Multiboot information */
    if (magic == 0x2BADB002) {
        vga_puts("[OK] Multiboot magic number: ");
        vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
        vga_print_hex(magic);
        vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
        vga_puts("\n");

        if (mboot_ptr != 0) {
            vga_puts("Memory (lower): ");
            vga_print_decimal(mboot_ptr->mem_lower);
            vga_puts(" KB\n");

            vga_puts("Memory (upper): ");
            vga_print_decimal(mboot_ptr->mem_upper);
            vga_puts(" KB\n");
        }
    } else {
        vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        vga_puts("[ERROR] Invalid Multiboot magic number!\n");
        vga_print_hex(magic);
        vga_puts("\n");
    }

    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_puts("\nKernel halted.\n");

    /* Hang the CPU */
    __asm__ volatile ("cli; hlt");
    while(1);
}
