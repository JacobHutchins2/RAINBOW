inlcude Makefile.inc

all:	kernel.img kernel.dis

kernel.img: kernel.elf
	$(CROSS)objcopy kernel.elf -O binary kernel.img

kernel.elf: kernel_main.o \

kernel_main.o: kernel_main.c
	$(CROSS)$(CC) $(CFLAGS) -o kernel_main.o -c kernel_main.c
