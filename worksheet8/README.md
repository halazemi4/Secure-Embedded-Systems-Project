# Worksheet 8

## Work attempted
- Built and flashed the default FreeRTOS LED task example.
- Configured the project to use the FreeRTOS source files.
- Updated the Makefile paths for FreeRTOS, Newlib, `libgcc`, and the ARM toolchain.
- Used `FreeRTOSConfig.h` to configure the FreeRTOS scheduler and Cortex-M3 system handlers.
- Created two FreeRTOS tasks using `xTaskCreate()`.
- Started the scheduler using `vTaskStartScheduler()`.
- Used `vTaskDelay()` to block tasks and allow the scheduler to run other tasks.
- Tested two LED tasks blinking at different speeds.
- Modified the LED blink delays to change the LED flashing rates.
- Changed task priorities to demonstrate task starvation.
- Created a starvation case where a higher-priority task does not block.
- Added two button tasks for the WKUP and TAMPER buttons.
- Used shared variables between button tasks and LED tasks to enable or disable LED blinking.
- Increased the FreeRTOS heap size to support additional tasks.
- Implemented a parameterized LED task using a structure containing the LED number and delay time.
- Created two LED tasks from the same task function using different parameters.
- Attempted the FreeRTOS maths game credit exercise using a maths task and a countdown clock task.
- Added USART2 support for the FreeRTOS maths game using PD5 as TX and PD6 as RX.
- Tested builds using OpenOCD, telnet, and the Olimex board.

## Work Demonstration
Screenshots and videos are stored in the screenshots directory.

## Files included

- `main.c`
- `main_freertos_maths_game.c` — FreeRTOS maths game credit exercise version.
- `main_parameterized_version.c` — Parameterized LED task credit exercise version.
- `main_shared_variable_version.c` — Shared variable button task version.
- `FreeRTOSConfig.h`
- `partest.c`
- `partest.h`
- `Makefile`
- `openocd.cfg`
- `startup_stm32f10x.c`
- `stm32f10x_conf.h`
- `stm32f100.ld`
- `demo.elf`
- `objects/`
- `screenshots/`
