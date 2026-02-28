CC = i686-elf-gcc
AS = i686-elf-as

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Isrc
LDFLAGS = -ffreestanding -O2 -nostdlib

OBJS = build/boot.o build/kernel.o build/vga_display.o
KERNEL = build/bsharp.bin

all: $(KERNEL)

$(KERNEL): $(OBJS) src/linker.ld
	$(CC) -T src/linker.ld -o $@ $(LDFLAGS) $(OBJS) -lgcc

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/vga_display.o: src/drivers/vga-display/vga_display.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.s
	@mkdir -p build
	$(AS) $< -o $@

run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

clean:
	rm -f build/*.o build/*.bin
