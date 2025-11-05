CROSS = arm-none-eabi-
CC = gcc
AS = as
LD = ld

# for pi model 1A/1B/1B+
ASFLAGS = -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard
CFLAGS  = -O2 -Wall -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marm -nostartfiles -ffreestanding -nostdlib -mgeneral-regs-only

# for newer pis
#ASFLAGS = -mcpu=cortex-a7
#CFLAGS = -Wall -O2 -mcpu=cortex-a7 -nostartfiles -ffreestanding -nostdlib

LFLAGS = --no-undefined -T $(LINKER_SCRIPT)

LINKER_SCRIPT = kernel.ld
