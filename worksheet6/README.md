# Worksheet 6

## Work attempted
- Built and configured Newlib for the ARM `arm-none-eabi` target.
- Linked the Worksheet 6 project with Newlib, `libc`, `libm`, and `libgcc`.
- Modified the Makefile to include Newlib paths and syscall support.
- Added `sys_calls.c` to provide the required Newlib system call stubs.
- Implemented `_write()` so `printf()` output is sent through USART.
- Tested `printf()` output using minicom.
- Implemented `_read()` so `scanf()` can receive input through USART.
- Tested serial input using `scanf()`.
- Added a local `strlen()` implementation to resolve the Newlib linking issue.
- Modified `_sbrk()` to use a direct Main Stack Pointer read instead of `core_cm3.o`.
- Created a maths test program using `srand()`, `rand()`, `printf()`, and `scanf()`.
- Added memory debug code to print data, BSS, heap, and stack-related addresses.
- Tested the program on the Olimex board using OpenOCD, telnet, and minicom.

## Work Demonstration
Screenshots are stored in the screenshots directory.

## Files included

- `main.c`
- `sys_calls.c`
- `com_port.h`
- `Makefile`
- `openocd.cfg`
- `startup_stm32f10x.c`
- `stm32f10x_conf.h`
- `stm32f100.ld`
- `demo.elf`
- `newlib_build_log.txt`
- `main_debug_version.c`  
