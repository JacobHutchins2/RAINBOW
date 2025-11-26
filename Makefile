include Makefile.inc

all:	kernel.img kernel.dis

kernel.img: kernel.elf
	$(CROSS)objcopy kernel.elf -O binary kernel.img

# 			dependencies then
#			linker links
kernel.elf: kernel_main.o \
			boot.o shell.o string.o i2c.o \
			serial.o printk.o act_led_init.o 
	$(CROSS)ld $(LFLAGS) kernel_main.o \
			boot.o shell.o string.o i2c.o \
			serial.o printk.o act_led_init.o \
			-Map kernel.map -o kernel.elf

kernel_main.o: kernel_main.c
	$(CROSS)$(CC) $(CFLAGS) -o kernel_main.o -c kernel_main.c

shell.o: shell.c shell.h
	$(CROSS)$(CC) $(CFLAGS) -o shell.o -c shell.c

boot.o: boot.s
	$(CROSS)as $(ASFLAGS) -o boot.o boot.s

string.o: string.c string.h
	$(CROSS)$(CC) $(CFLAGS) -o string.o -c string.c

serial.o: serial.c bcm2835_addr.h serial.h
	$(CROSS)$(CC) $(CFLAGS) -o serial.o -c serial.c

printk.o: printk.c printk.h
	$(CROSS)$(CC) $(CFLAGS) -o printk.o -c printk.c

i2c.o: i2c.c bcm2835_addr.h i2c.h
	$(CROSS)$(CC) $(CFLAGS) -o i2c.o -c i2c.c

act_led_init.o: act_led_init.c act_led_init.h
	$(CROSS)$(CC) $(CFLAGS) -o act_led_init.o -c act_led_init.c


kernel.dis:	kernel.elf
	$(CROSS)objdump --disassemble-all kernel.elf > kernel.dis


clean:
	rm -f *~ *.o *.map *.elf kernel.img *.dis
