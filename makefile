MCU      = cortex-m4
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
AS       = arm-none-eabi-as

CFLAGS = -mcpu=$(MCU) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
         -Os -Wall -nostdlib -g \
         -DSTM32F446xx \
         -Iinclude

LDFLAGS = -T linker.ld -lc -lgcc --specs=nosys.specs

SRCS = src/app/main.c src/app/init.c src/startup/system_stm32f4xx.c
STARTUP  = src/startup/startup_stm32f446xx.s

all: build/main.bin

build/main.elf: $(SRCS) $(STARTUP)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

build/main.bin: build/main.elf
	$(OBJCOPY) -O binary $< $@

flash: build/main.bin
	st-flash write build/main.bin 0x8000000

clean:
	rm -f build/*.elf build/*.bin build/compile_commands.json
