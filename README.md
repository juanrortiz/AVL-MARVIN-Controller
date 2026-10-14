# MARVIN Embedded Controller — Electronics Hardware Design

**CPP AVL | Fall Cohort 2026 | Intelligent Ground Vehicle Competition (IGVC)**

This repository contains the hardware design files and embedded firmware source for a custom controller developed for **MARVIN**, the CPP Autonomous Vehicle Lab (AVL) competition platform.

The controller is built around the **STM32F767ZIT6** microcontroller and is intended to modernize MARVIN's motor-control electronics, replacing the existing **Teensy 4.1**-based controller with an STM32-based system.

## Project Goals

During the Fall 2026 cohort, the Electronics Hardware Design Team aims to:

- Design and integrate a new STM32-based embedded control system for MARVIN.
- Migrate CAN-based motor control from the existing Teensy 4.1 controller.
- Support the currently used **SPARK MAX** motor drivers over CAN.
- Improve compatibility with future **ODrive S Series** CAN motor drivers.
- Standardize the control hardware around an industry-common STM32 platform.
- Validate reliable communication, power monitoring, and operation on the vehicle.

## Hardware Overview

The custom controller integrates the following components and subsystems:

| Subsystem | Description |
| --- | --- |
| Main MCU | STM32F767ZIT6 |
| CAN Interface | CAN transceiver for communication with motor drivers |
| USB / Serial | Serial-to-USB interface for communication and debugging |
| Power Monitoring | Onboard power-monitoring circuitry |
| PCB Design | KiCad schematic and PCB layout files |

> **Note:** Electrical specifications, connector pinouts, CAN configuration, and power limits should be verified against the latest KiCad design before hardware is assembled or connected.

## Repository Contents

This repository includes:

- **KiCad project files** — schematics, PCB layouts, and related hardware design assets.
- **Firmware source code** — embedded application code targeting the STM32F767ZIT6.
- **Makefile** — firmware build configuration.
- **Linker scripts** — memory layout and linking configuration for the STM32 target.

The exact directory layout may evolve as the project develops.

## Firmware Dependencies

The firmware depends on official **STMicroelectronics STM32 HAL and CMSIS libraries**. These libraries are **not necessarily bundled with this repository** and must be obtained separately from the official STM32 software packages.

Recommended sources:

- [STM32CubeF7 firmware package](https://github.com/STMicroelectronics/STM32CubeF7)
- [STM32F7 HAL drivers](https://github.com/STMicroelectronics/stm32f7xx-hal-driver)
- [STM32Cube ecosystem](https://www.st.com/en/embedded-software/stm32cube-mcu-mpu-packages.html)

Ensure the HAL, CMSIS, startup files, include paths, and linker configuration match the **STM32F767ZIT6** and the project's Makefile. Dependency setup and build commands will be documented as the firmware structure is finalized.

## Development Roadmap

- [ ] Finalize and review the KiCad schematic and PCB design.
- [ ] Assemble and bring up the STM32F767ZIT6 controller hardware.
- [ ] Configure the clock tree, GPIO, CAN, serial/USB, and power-monitoring peripherals.
- [ ] Integrate official STM32 HAL/CMSIS dependencies into the firmware build.
- [ ] Implement and validate CAN communication with SPARK MAX motor drivers.
- [ ] Migrate motor-control functionality from Teensy 4.1 to STM32.
- [ ] Evaluate and implement compatibility with ODrive S Series CAN motor drivers.
- [ ] Perform bench testing, vehicle integration, and system validation.

*Checklist items represent project objectives, not confirmed implementation status.*

## Development and Testing Notes

Before connecting motor drivers or installing the controller on MARVIN:

1. Verify PCB power rails, grounding, and connector pinouts.
2. Confirm CAN bus voltage levels, termination, bitrate, and device-specific message protocols.
3. Test communication and power monitoring on the bench before enabling motor outputs.
4. Validate fault handling and emergency-stop behavior as part of vehicle integration.

**Important:** SPARK MAX and ODrive motor controllers may use different CAN protocols. Electrical CAN compatibility alone does not guarantee firmware-level interoperability; each device requires the appropriate protocol implementation.

## Team

**CPP Autonomous Vehicle Lab (AVL)**  
**Fall Cohort 2026 — MARVIN Electronics Hardware Design Team**  
**Competition:** Intelligent Ground Vehicle Competition (IGVC)

## Project Status

**In development — Fall 2026 cohort.** Hardware, firmware, and integration details are subject to change as the controller is developed and validated.
