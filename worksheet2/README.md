# Worksheet 2

## Work attempted
- Built the original `main.c` program using the supplied Makefile.
- Checked the ARM cross-compilation process.
- Connected to the Olimex STM32-P107 board using OpenOCD.
- Loaded `demo.elf` onto the board using telnet/OpenOCD.
- Debugged the program using GDB.
- Added a new variable `k` to `main.c`.
- Rebuilt, reloaded and debugged the modified program.
- Used breakpoints on `main` and `inc`.
- Used a GDB watchpoint on a variable.
- Printed register values using GDB.
- Used breakpoint commands to print `i` automatically and continue.
- Answered the worksheet questions about ARM devices, GCC flags, compilation stages, Makefile labels, linker scripts and startup code in the attached word file.

## Work Demonstratioin
Screenshots are stored in the `screenshots/` directory.

## Files
- `main.c`
- `Makefile`
- `openocd.cfg`
- `startup_stm32f10x.c`
- `stm32f10x_conf.h`
- `stm32f100.ld`
- `demo.elf` generated during build
