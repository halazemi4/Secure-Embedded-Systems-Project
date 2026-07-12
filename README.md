# Secure Embedded Systems Worksheets

## Author: Hamad Alazemi
# ID: 23081287

## Project overview
This repository contains the completed Secure Embedded Systems worksheet work for the Olimex STM32-P107 / Cortex-M3 board. The work covers GPIO control, serial communication, Newlib integration, interrupts, timers, PWM, and FreeRTOS task scheduling.

Each worksheet folder contains the source files, Makefile, board configuration files, screenshots/videos, and a worksheet-specific README explaining what was attempted and demonstrated.

## Hardware and tools used
- Olimex STM32-P107 / STM32F107 Cortex-M3 board
- ARM GCC toolchain: `arm-none-eabi-gcc`
- OpenOCD for flashing and debugging
- Telnet connection to OpenOCD on port `4444`
- Minicom for USART serial testing
- STM32F10x Standard Peripheral Library
- Newlib for Worksheet 6
- FreeRTOS for Worksheet 8

## Repository structure

```text
SES/
├── worksheet2/
├── worksheet3/
├── worksheet4/
├── worksheet6/
├── worksheet7/
├── worksheet8/
├── STM32F10x_StdPeriph_Lib_V3.5.0/
├── newlib-arm-none-eabi/
├── Questions/
└── README.md
```

## Worksheets included

### Worksheet 2
Introduces basic embedded C project structure, Makefile building, flashing the Olimex board, and simple GPIO output testing.

### Worksheet 3
Covers button input and LED output using GPIO. The work includes testing the board buttons and using them to control LEDs.

### Worksheet 4
Covers USART serial communication. The work includes configuring USART2 on PD5 and PD6, sending output to Minicom, receiving input, echoing characters, and attempting receive error handling.

### Worksheet 6
Covers building and linking with Newlib. The work includes configuring the Makefile for Newlib, implementing system calls such as `_write()`, `_read()`, and `_sbrk()`, testing `printf()` and `scanf()`, creating a maths quiz, and printing memory debug information for data, stack, and heap values.

### Worksheet 7
Covers interrupts, timers, USART interrupts, and PWM. The work includes TIM2 timer interrupts, EXTI button interrupts, USART interrupt-driven input/output, circular RX/TX buffers, and PWM fading of green and yellow LEDs in opposite directions.

### Worksheet 8
Covers FreeRTOS on the Cortex-M3. The work includes creating FreeRTOS tasks, using `vTaskDelay()`, changing blink speeds, demonstrating task starvation, using shared variables between button and LED tasks, creating a parameterized LED task, and attempting a FreeRTOS maths game with a countdown task.

## Attempted questions
Each attempted question is answered in a word file inside of /Questions folders
 
## Evidence
Each worksheet folder contains screenshots and videos showing the code building, flashing, and running on the board.
