MCU      = cortex-m7
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy

STM32_DIR    = $(HOME)/Embedded/cmsis-device-f7
CMSIS_DIR    = $(HOME)/Embedded/CMSIS_6/CMSIS
HAL_DIR      = $(HOME)/Embedded/stm32f7xx-hal-driver

CFLAGS = \
    -mcpu=$(MCU) \
    -mthumb \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -Os \
    -Wall \
    -g \
    -ffunction-sections \
    -fdata-sections \
    -DSTM32F767xx \
    -DCMSIS_device_header=\"stm32f767xx.h\" \
    -Iinclude \
    -I$(STM32_DIR)/Include \
    -I$(CMSIS_DIR)/Core/Include \
    -I$(HAL_DIR)/Inc

HAL_SRCS = \
    $(HAL_DIR)/Src/stm32f7xx_hal.c \
    $(HAL_DIR)/Src/stm32f7xx_hal_cortex.c \
    $(HAL_DIR)/Src/stm32f7xx_hal_gpio.c \
    $(HAL_DIR)/Src/stm32f7xx_hal_rcc.c \
    $(HAL_DIR)/Src/stm32f7xx_hal_uart.c \
    $(HAL_DIR)/Src/stm32f7xx_hal_can.c

LDFLAGS = \
    -T linker.ld \
    --specs=nano.specs \
    --specs=nosys.specs \
    -Wl,--gc-sections \
    -Wl,-Map=build/main.map

SRCS = \
	src/app/main.c \
	src/app/init.c \
	src/app/can_app.c \
	src/startup/system_stm32f7xx.c \
	$(HAL_SRCS)

STARTUP = src/startup/startup_stm32f767xx.s

all: build/main.bin

build/main.elf: $(SRCS) $(STARTUP)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

build/main.bin: build/main.elf
	$(OBJCOPY) -O binary $< $@

flash: build/main.bin
	st-flash write build/main.bin 0x8000000

clean:
	rm -f build/*.elf build/*.bin build/compile_commands.json
