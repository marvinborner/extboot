// MIT License, Copyright (c) 2021 Marvin Borner
// Example kernel

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

struct vid_info {
	u32 mode;
	u32 *vbe;
};

enum mmap_type {
	MEMORY_AVAILABLE = 1,
	MEMORY_RESERVED,
	MEMORY_ACPI,
	MEMORY_NVS,
	MEMORY_DEFECT,
	MEMORY_DISABLED
};

struct mmap_boot {
	u32 lbase;
	u32 hbase;
	u32 lsize;
	u32 hsize;
	u32 type;
	u32 acpi;
};

struct mem_info {
	struct mmap_boot *start;
	u32 *end;
	u32 size;
};

struct boot_info {
	struct vid_info *vid;
	struct mem_info *mem;
	u32 tss;
	u32 drive;
};

struct vbe_basic {
	u8 stuff1[16];
	u16 pitch;
	u16 width;
	u16 height;
	u8 stuff2[18];
	u8 *fb;
	u8 stuff3[212];
};

int kernel_main(struct boot_info *boot);
int kernel_main(struct boot_info *boot)
{
	// Fill screen with rgba colors
	u32 colors[7] = { 0xff0000, 0xffa500, 0xffff00, 0x008000, 0x0000ff, 0x4b0082, 0xee82ee };
	struct vbe_basic *vbe = (void *)boot->vid->vbe;
	u8 *draw = vbe->fb;
	for (u32 y = 0; y < vbe->height; y++) {
		u32 color = colors[y / (vbe->height / 7)];
		for (u32 x = 0; x < vbe->width; x++) {
			draw[4 * x] = color >> 0 & 0xff;
			draw[4 * x + 1] = color >> 8 & 0xff;
			draw[4 * x + 2] = color >> 16 & 0xff;
			draw[4 * x + 3] = 0xff; // Alpha is always 0xff in this example
		}
		draw += vbe->pitch;
	}

	// Idle
	__asm__ volatile("cli");
	while (1)
		__asm__ volatile("hlt");

	return 1;
}
