#include "vga_display.h"

/* VGA terminal state */
static uint16_t *vga_buffer = (uint16_t *)VGA_MEMORY;
static size_t vga_row = 0;
static size_t vga_col = 0;
static enum vga_color vga_color = VGA_COLOR_WHITE;
static enum vga_color vga_bg_color = VGA_COLOR_BLACK;

/* Helper function to combine foreground and background colors */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | (bg << 4);
}

/* Helper function to create a VGA entry (character + color) */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return ((uint16_t)uc) | (((uint16_t)color) << 8);
}

/* Initialize the VGA text mode terminal */
void vga_init(void) {
    vga_row = 0;
    vga_col = 0;
    vga_color = VGA_COLOR_WHITE;
    vga_bg_color = VGA_COLOR_BLACK;

    uint8_t color = vga_entry_color(vga_color, vga_bg_color);
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', color);
    }
}

/* Scroll the terminal up by one line */
static void vga_scroll(void) {
    uint8_t color = vga_entry_color(vga_color, vga_bg_color);

    /* Shift all lines up */
    for (size_t i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
        vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
    }

    /* Clear the last line */
    for (size_t i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', color);
    }
}

/* Move to the next line and handle scrolling */
static void vga_newline(void) {
    vga_col = 0;
    vga_row++;

    if (vga_row >= VGA_HEIGHT) {
        vga_scroll();
        vga_row = VGA_HEIGHT - 1;
    }
}

/* Put a single character on the screen */
void vga_putchar(char c) {
    uint8_t color = vga_entry_color(vga_color, vga_bg_color);

    if (c == '\n') {
        vga_newline();
        return;
    }

    if (c == '\t') {
        /* Handle tabs as 4 spaces */
        for (int i = 0; i < 4; i++) {
            vga_putchar(' ');
        }
        return;
    }

    if (c == '\r') {
        vga_col = 0;
        return;
    }

    size_t index = vga_row * VGA_WIDTH + vga_col;
    vga_buffer[index] = vga_entry((unsigned char)c, color);

    vga_col++;
    if (vga_col >= VGA_WIDTH) {
        vga_newline();
    }
}

/* Print a null-terminated string */
void vga_puts(const char *str) {
    while (*str) {
        vga_putchar(*str);
        str++;
    }
}

/* Set the text color for subsequent output */
void vga_set_color(enum vga_color fg, enum vga_color bg) {
    vga_color = fg;
    vga_bg_color = bg;
}

/* Simple integer to string conversion for debugging */
static void vga_put_number(uint32_t num, int base, int uppercase) {
    static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    static const char DIGITS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *d = uppercase ? DIGITS : digits;

    if (num == 0) {
        vga_putchar('0');
        return;
    }

    char buffer[32];
    int i = 0;

    while (num > 0) {
        buffer[i++] = d[num % base];
        num /= base;
    }

    while (i > 0) {
        vga_putchar(buffer[--i]);
    }
}

/* Print a number in decimal */
void vga_print_decimal(uint32_t num) {
    vga_put_number(num, 10, 0);
}

/* Print a number in hexadecimal */
void vga_print_hex(uint32_t num) {
    vga_puts("0x");
    vga_put_number(num, 16, 0);
}

/* Clear the screen */
void vga_clear(void) {
    vga_init();
}
