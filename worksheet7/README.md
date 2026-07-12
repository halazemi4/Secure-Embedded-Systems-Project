# Worksheet 7

## Work attempted
- Built and flashed the timer interrupt example.
- Configured TIM2 to generate update interrupts.
- Used `TIM2_IRQHandler()` to toggle the green and yellow LEDs.
- Changed timer values to control the LED flashing speed.
- Built and tested the button interrupt example.
- Configured the WKUP button on PA0 using EXTI line 0.
- Used `EXTI0_IRQHandler()` to toggle the yellow LED.
- Modified the button interrupt program to use the GPIOC button.
- Configured PC13 using EXTI line 13 and `EXTI15_10_IRQHandler()`.
- Used the GPIOC button interrupt to toggle the green LED.
- Built and tested USART2 interrupt-driven input.
- Configured USART2 using PD5 as TX and PD6 as RX.
- Enabled USART2 receive interrupts using `USART_IT_RXNE`.
- Used `USART2_IRQHandler()` to receive characters and echo them back.
- Implemented full interrupt-driven serial IO using RX and TX circular buffers.
- Created 20-character receive and transmit buffers.
- Implemented `outbyte()` to place characters into the transmit buffer.
- Implemented `inbyte()` to read characters from the receive buffer.
- Modified the USART2 interrupt handler to handle both receive and transmit interrupts.
- Disabled the TX interrupt when there were no characters left to send.
- Tested serial echo using minicom.
- Implemented PWM fading using TIM3.
- Configured PC6 and PC7 as alternate function outputs for PWM.
- Used TIM3 channel 1 for the green LED and TIM3 channel 2 for the yellow LED.
- Implemented the credit exercise where the green and yellow LEDs fade in opposite directions.

## Work Demonstration
Screenshots and videos are stored in the screenshots directory.

## Files included

- `main.c`
- `TimerInt.c`
- `buttona.c`
- `Makefile`
- `openocd.cfg`
- `startup_stm32f10x.c`
- `stm32f10x_conf.h`
- `stm32f100.ld`
- `demo.elf`
- `main_interrupt_driven_IO_version.c`
- `main_PWM_version.c`
