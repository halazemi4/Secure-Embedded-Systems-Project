# Worksheet 3

## Work attempted
- Configured GPIOC and flashed the green LED using GPIO_WriteBit.
- Replaced GPIO_WriteBit with direct register access using GPIOC->BSRR and GPIOC->BRR.
- Compared library and direct-register build sizes using arm-none-eabi-size.
- Configured PC6 and PC7 to alternately flash the green and yellow LEDs.
- Configured PA0 as an input for the WKUP button.
- Used WKUP to toggle the green LED.
- Used WKUP to toggle between green and yellow LEDs.
- Implemented a debounce finite state machine for reliable button input.

## Work Demonstration
Screenshots and videos are stored in the screenshots directory.

## Files included

-   `main.c` 
-   `Makefile` 
-   `openocd.cfg` 
-   `startup_stm32f10x.c` 
-   `stm32f10x_conf.h` 
-   `stm32f100.ld` 
-   `demo.elf` 
