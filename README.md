# Template STM32 Project

Bare-metal STM32 project template — CMSIS registers, no HAL, Makefile-based build.

## Project structure

```
.
├── build/              # compile_commands.json, main.elf, main.bin (generated)
├── config/
│   └── recover.cfg     # OpenOCD recovery config
├── include/             # CMSIS headers (see "CMSIS setup" below)
├── linker.ld            # Flash/RAM memory map
├── makefile
└── src/
    ├── app/
    │   └── main.c
    └── startup/
        ├── startup_stm32fXXXxx.s
        └── system_stm32fXxx.c
```

## Toolchain

```bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-newlib arm-none-eabi-gdb
sudo pacman -S openocd stlink
sudo pacman -S picocom bear
```

## CMSIS setup

CMSIS headers aren't packaged — clone and copy them into `include/` on a new project:

```bash
git clone https://github.com/STMicroelectronics/cmsis-device-f4.git
git clone https://github.com/ARM-software/CMSIS_5.git

cp cmsis-device-f4/Include/*.h include/
cp CMSIS_5/CMSIS/Core/Include/core_cm4.h include/
cp CMSIS_5/CMSIS/Core/Include/cmsis_gcc.h include/
cp CMSIS_5/CMSIS/Core/Include/cmsis_version.h include/
cp CMSIS_5/CMSIS/Core/Include/mpu_armv7.h include/

cp cmsis-device-f4/Source/Templates/system_stm32f4xx.c src/startup/
cp cmsis-device-f4/Source/Templates/gcc/startup_stm32fXXXxx.s src/startup/
```

Swap `cmsis-device-f4` for the matching family repo if targeting a different STM32 line.

## Makefile — per-project settings

Update these when starting a new project from this template:

- `-DSTM32FXXXxx` — target MCU define (must match the CMSIS header)
- `-mcpu=` — core type (e.g. `cortex-m4`)
- `linker.ld` — flash/RAM size and origin for the target chip
- `SRCS` — source file list
- `STARTUP` — path to the matching startup `.s` file

## Build & flash

```bash
make clean && bear --output build/compile_commands.json -- make
make flash
```

## Debug

```bash
openocd -f config/recover.cfg
arm-none-eabi-gdb build/main.elf
(gdb) target remote :3333
(gdb) monitor reset halt
(gdb) load
(gdb) continue
```

## Serial monitor

```bash
picocom -b 115200 --flow n /dev/ttyACM0     # adjust port as needed
```

## Recovery

If `config/recover.cfg` is missing, recreate it with:

```bash
echo 'source [find interface/stlink.cfg]
source [find target/stm32f4x.cfg]
reset_config srst_only srst_nogate
adapter speed 480' > config/recover.cfg
```

Mass erase (use if the board is unresponsive / bad flash state):

```bash
openocd -f config/recover.cfg -c "init; reset halt; stm32f4x mass_erase 0; exit"
```

Once complete, reflash normally:

```bash
make flash
```

## Git push

### Authenticate GitHub

```bash
sudo pacman -S github-cli
gh auth login
```

### Create the repo on GitHub, then init your local folder

```bash
git init
git remote add origin https://github.com/USERNAME/REPO-NAME.git
```

```bash
git add .
git commit -m "Initial commit"
git branch -M main
git push -u origin main
```

### Note

`git remote set-url` only works on an existing remote — use `git remote add` for a fresh repo.

```bash
git remote set-url origin https://github.com/USERNAME/REPO-NAME.git
```
