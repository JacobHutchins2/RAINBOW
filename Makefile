include Makefile.inc

all:	kernel.img 

kernel.img: kernel.elf
	$(CROSS)objcopy kernel.elf -O binary kernel.img

# 			dependencies then
#			linker links
kernel.elf: kernel_main.o \
			boot.o shell.o string.o i2c.o \
			serial.o printk.o act_led_init.o \
			shell_commands.o interrupts.o timer.o \
			syscalls.o display_io.o spi.o gpio.o tft.o \
			buttons.o pwm.o font.o ili9341.o ui.o
	$(CROSS)ld $(LFLAGS) kernel_main.o \
			boot.o shell.o string.o i2c.o \
			serial.o printk.o act_led_init.o \
			shell_commands.o interrupts.o timer.o \
			syscalls.o display_io.o spi.o gpio.o tft.o \
			buttons.o pwm.o font.o ili9341.o ui.o \
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

shell_commands.o: shell_commands.c shell_commands.h
	$(CROSS)$(CC) $(CFLAGS) -o shell_commands.o -c shell_commands.c

timer.o: timer.c bcm2835_addr.h timer.h 
	$(CROSS)$(CC) $(CFLAGS) -o timer.o -c timer.c

interrupts.o: interrupts.c bcm2835_addr.h interrupts.h
	$(CROSS)$(CC) $(CFLAGS) -o interrupts.o -c interrupts.c

syscalls.o: syscalls.c syscalls.h
	$(CROSS)$(CC) $(CFLAGS) -o syscalls.o -c syscalls.c

display_io.o: display_io.c display_io.h
	$(CROSS)$(CC) $(CFLAGS) -o display_io.o -c display_io.c

spi.o: spi.c spi.h
	$(CROSS)$(CC) $(CFLAGS) -o spi.o -c spi.c

gpio.o: gpio.c gpio.h
	$(CROSS)$(CC) $(CFLAGS) -o gpio.o -c gpio.c

tft.o: tft.c tft.h
	$(CROSS)$(CC) $(CFLAGS) -o tft.o -c tft.c

pwm.o: pwm.c pwm.h
	$(CROSS)$(CC) $(CFLAGS) -o pwm.o -c pwm.c

buttons.o: buttons.c buttons.h
	$(CROSS)$(CC) $(CFLAGS) -o buttons.o -c buttons.c

font.o: font.c font.h
	$(CROSS)$(CC) $(CFLAGS) -o font.o -c font.c

ili9341.o: ili9341.c ili9341.h
	$(CROSS)$(CC) $(CFLAGS) -o ili9341.o -c ili9341.c

ui.o: ui.c ui.h
	$(CROSS)$(CC) $(CFLAGS) -o ui.o -c ui.c


clean:
	rm -f *~ *.o *.map *.elf kernel.img *.dis
