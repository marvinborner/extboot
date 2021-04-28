# extboot

This repository is the home of my self-written i386 bootloader I initially created for my hobby operating system [Melvix](https://github.com/marvinborner/Melvix) (it uses other bootloaders nowadays). It supports ATA disk detection, EXT2 loading, basic ELF parsing/mapping and much more!

## Disclaimer

This bootloader will **not** work with any common operating system. Its main usecase is the booting of custom **hobby** operating systems. Even then, this bootloader doesn't support modern hardware features (AHCI, XHCI, APIC, ACPI, UEFI, ...). If you seek a bootloader supporting Linux, Windows or in general modern hardware, you may find Limine, Grub and similar alternatives more interesting.

## Example usage

This repository includes a very basic example. You can find a more advanced implementation [here (Melvix snapshot)](https://github.com/marvinborner/Melvix/tree/895a58b1b57a0ae8028576404a90f12e0133cf5f). You'll probably want to use GNU/Linux or OpenBSD if you want to try/build this example.

### Test prebuilt by CI

-   Install the QEMU i386 emulator
-   Download the `disk-img` artifact from [GitHub Workflow build](https://github.com/marvinborner/extboot/actions)
-   Unzip `disk-img.zip`
-   Run `qemu-system-i386 -serial stdio -m 256M -vga std -drive file=disk.img,format=raw,index=1,media=disk`

### Build

You can build and run it in the virtual machine QEMU using `./run example` (this will compile a 32 bit x86 cross compiler first and will take some time). You'll need the following build dependencies:

-   General template: `[pkg_manager] [install] git binutils gcc make bison flex gmp mpc mpfr texinfo curl nasm qemu` (package names may vary depending on your operating system)
-   Ubuntu/Debian: `apt-get install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo curl nasm grub-common qemu qemu-kvm mtools`
-   OpenBSD: `pkg_add git gcc g++ gmake bison gmp libmpc mpfr texinfo curl nasm qemu e2fsprogs`

## Execution sequence

The bootloader starts in the first stage at `0x7c00` - which is the location of the MBR - by clearing the screen, finding the disk, checking for basic addressing support and finally loading the second stage. The first stage has a max size of 512 bytes, thanks to some ancient BIOS standards. That's why not much is happening in here.

The second stage searches and loads the third stage (`load.c`) using a very minimal ext2 implementation (still in assembly!). The second stage also loads the memory map and the VESA video map into memory and sets the highest available video mode. After the A20 line has been enabled and the GDT and TSS have been set, it's time to jump to the protected mode. After the computer arrived in protected mode, the third stage gets executed, with the memory and VESA map as its parameters. Finally.

The third stage (`load.c`) installs a basic serial connection for logging purposes and probes ATA disk drives. After a suitable disk has been found, the third stage will load the kernel binary into memory using an EXT2 driver and basic ELF parsing/mapping. Ultimately, the bootloader will jump to the kernel. The rest is history.
